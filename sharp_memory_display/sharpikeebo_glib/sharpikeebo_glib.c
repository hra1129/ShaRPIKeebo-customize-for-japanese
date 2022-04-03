// --------------------------------------------------------------------
// Sharpikeebo game library
// ====================================================================
//	Copyright 2022 t.hara
//
//	Permission is hereby granted, free of charge, to any person obtaining 
//	a copy of this software and associated documentation files (the "Software"), 
//	to deal in the Software without restriction, including without limitation 
//	the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//	and/or sell copies of the Software, and to permit persons to whom the 
//	Software is furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
//	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
//	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
//	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
//	DEALINGS IN THE SOFTWARE.
// --------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <pthread.h>
#include <armbianio.h>
#include "sharpikeebo_glib.h"

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
	// semun is defined into <sys/sem.h>. */
#else
union semun {
	int val;					// value for SETVAL
	struct semid_ds *buf;		// buffer for IPC_STAT, IPC_SET
	unsigned short int *array;	// array for GETALL, SETALL
	struct seminfo *__buf;		// buffer for IPC_INFO
};
#endif

#define spk_width		400
#define spk_height		240

#define CS				16
#define DISPON			18
#define EXTCOMIN		22
#define SPEED			3500000

#define LED0			15				//	LEFT-UP
#define LED1			37				//	RIGHT-UP
#define LED2			35				//	LEFT-DOWN
#define LED3			33				//	RIGHT-DOWN

#define SW_LEFT			12				//	BUT1	SW102
#define SW_BUTTON		32				//	BUT2	SW101
#define SW_DOWN			36				//	BUT3	SW103
#define SW_RIGHT		38				//	BUT4	SW104
#define SW_UP			40				//	BUT5	SW105

static int sem_id				= -1;	//	for SharpMemoryDisplay lock semaphore
static int hspi					= -1;
static pthread_t h_draw_thread;

static int update_sem_id		= -1;	//	for SharpMemoryDisplay update lock semaphore
static int info_sem_id			= -1;
static volatile int exit_request = 0;	//	0: normal, 1: exit request
static SPK_BACKBUFFER_T *p_send_buffer;

static const int led_pin[] = {
	LED0, LED1, LED2, LED3
};

static struct sembuf lock_operations;
static struct sembuf unlock_operations;

// --------------------------------------------------------------------
//	Table for inverting the sequence of bits
//	{ b7, b6, ... , b0 } --> { b0, b1, ... , b7 }
//
static const unsigned char mirror[256] = {
	0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240,
	8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
	4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
	12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
	2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
	10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
	6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
	14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
	1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241,
	9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
	5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
	13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
	3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243,
	11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
	7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
	15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255
};

// --------------------------------------------------------------------
static void *_draw_thread( void *p_arg ) {
	static uint8_t frame_buffer[ 1 + (spk_width / 8) + 1 ];
	int y, x, bit;
	uint8_t d = 0;
	const uint8_t *p;

	for(;;) {
		//	get semaphore (update request)
		semop( update_sem_id, &lock_operations, 1 );
		semop( info_sem_id, &lock_operations, 1 );
		if( exit_request ) break;
		//	convert to 1bpp from 8bpp.
		AIOWriteGPIO( CS, 1 );
		p = p_send_buffer->image;
		frame_buffer[0] = 0x80;
		AIOWriteSPI( hspi, frame_buffer, 1 );
		for( y = 1; y <= spk_height; y++ ) {
			frame_buffer[0] = mirror[ y ];
			for( x = 1; x < 51; x++ ) {
				for( bit = 0; bit < 8; bit++ ) {
					d = (d << 1) + ((*p) < 128);
					p++;
				}
				frame_buffer[x] = d;
			}
			frame_buffer[51] = 0;
			AIOWriteSPI( hspi, frame_buffer, 1 + 50 + 1 );
		}
		//	transfer
		AIOWriteGPIO( CS, 0 );
		semop( info_sem_id, &unlock_operations, 1 );
	}
	semop( info_sem_id, &unlock_operations, 1 );
	pthread_exit( NULL );
}

// --------------------------------------------------------------------
int spk_initialize( void ) {
	key_t key;
	union semun sem_arg;

	lock_operations.sem_num = 0;
	lock_operations.sem_op = -1;
	lock_operations.sem_flg = SEM_UNDO;
	unlock_operations.sem_num = 0;
	unlock_operations.sem_op = 1;
	unlock_operations.sem_flg = SEM_UNDO;

	AIOInitBoard( "Raspberry Pi" );

	AIOAddGPIO( LED0		, GPIO_OUT );
	AIOAddGPIO( LED1		, GPIO_OUT );
	AIOAddGPIO( LED2		, GPIO_OUT );
	AIOAddGPIO( LED3		, GPIO_OUT );

	AIOAddGPIO( SW_UP		, GPIO_IN );
	AIOAddGPIO( SW_LEFT		, GPIO_IN );
	AIOAddGPIO( SW_RIGHT	, GPIO_IN );
	AIOAddGPIO( SW_DOWN		, GPIO_IN );
	AIOAddGPIO( SW_BUTTON	, GPIO_IN );

	AIOAddGPIO( CS			, GPIO_OUT );
	AIOAddGPIO( DISPON		, GPIO_OUT );
	AIOAddGPIO( EXTCOMIN	, GPIO_OUT );

	//	Get SPI handle
	hspi = AIOOpenSPI( SPI_BUS_NUMBER, SPEED );
	if( hspi == -1 ) {
		return 0;
	}

	key = ftok( "/usr/local/bin/sharpikeebo_lcd", 1 );

	sem_id = semget( key, 1, 0666 );
	if( sem_id != -1 ) {
		semop( sem_id, &lock_operations, 1 );
	}

	update_sem_id = semget( IPC_PRIVATE, 1, S_IRUSR | S_IWUSR );
	if( update_sem_id == -1 ) {
		return 0;
	}
    sem_arg.val = 0;	//	locked
    semctl( update_sem_id, 0, SETVAL, sem_arg );

	info_sem_id = semget( IPC_PRIVATE, 1, S_IRUSR | S_IWUSR );
	if( info_sem_id == -1 ) {
		return 0;
	}
    sem_arg.val = 1;	//	unlocked
    semctl( info_sem_id, 0, SETVAL, sem_arg );

	AIOWriteGPIO( DISPON	, 1 );
	AIOWriteGPIO( EXTCOMIN	, 0 );
	AIOWriteGPIO( CS		, 0 );

	AIOWriteGPIO( LED0		, SPK_LED_OFF );
	AIOWriteGPIO( LED1		, SPK_LED_OFF );
	AIOWriteGPIO( LED2		, SPK_LED_OFF );
	AIOWriteGPIO( LED3		, SPK_LED_OFF );

	p_send_buffer = spk_get_backbuffer( spk_width, spk_height );
	if( p_send_buffer == NULL ) {
		return 0;
	}
	pthread_create( &h_draw_thread, NULL, _draw_thread, NULL );
	return 1;
}

// --------------------------------------------------------------------
void spk_terminate( void ) {
	void *p_result;

	exit_request = 1;
	semop( update_sem_id, &lock_operations, 1 );
	pthread_join( h_draw_thread, &p_result );
	if( sem_id != -1 ) {
		semop( sem_id, &unlock_operations, 1 );
	}
}

// --------------------------------------------------------------------
void spk_display( const SPK_BACKBUFFER_T *p_image ) {

	if( p_image->width != spk_width || p_image->height != spk_height ) {
		return;
	}
	semop( info_sem_id, &lock_operations, 1 );
	memcpy( p_send_buffer->image, p_image->image, spk_width * spk_height );
	semop( info_sem_id, &unlock_operations, 1 );
	semop( update_sem_id, &unlock_operations, 1 );
}

// --------------------------------------------------------------------
SPK_BACKBUFFER_T *spk_get_backbuffer( uint32_t width, uint32_t height ) {
	uint32_t size;
	SPK_BACKBUFFER_T *p;

	size = width * height;
	p = (SPK_BACKBUFFER_T*) malloc( sizeof(SPK_BACKBUFFER_T) + size - 1 );
	if( p != NULL ) {
		p->width	= width;
		p->height	= height;
		memset( p->image, 0, size );
	}
	return p;
}

// --------------------------------------------------------------------
SPK_BACKBUFFER_T *spk_get_display( void ) {

	return spk_get_backbuffer( spk_width, spk_height );
}

// --------------------------------------------------------------------
void spk_release_backbuffer( SPK_BACKBUFFER_T *p_image ) {
	free( p_image );
}

// --------------------------------------------------------------------
void spk_clear_buffer( SPK_BACKBUFFER_T *p_image, uint8_t c ) {

	memset( p_image->image, c, p_image->width * p_image->height );
}

// --------------------------------------------------------------------
void spk_set_pixel( SPK_BACKBUFFER_T *p_image, int x, int y, uint8_t c ) {

	if( x < 0 || y < 0 || x >= p_image->width || y >= p_image->height ) {
		return;
	}
	p_image->image[ x + y * p_image->width ] = c;
}

// --------------------------------------------------------------------
uint8_t spk_get_pixel( SPK_BACKBUFFER_T *p_image, int x, int y ) {

	if( x < 0 || y < 0 || x >= p_image->width || y >= p_image->height ) {
		return 0;
	}
	return p_image->image[ x + y * p_image->width ];
}

// --------------------------------------------------------------------
void spk_line( SPK_BACKBUFFER_T *p_image, int x1, int y1, int x2, int y2, uint8_t c ) {
	int w, h, vx, vy, n, i;

	if( x1 == x2 && y1 == y2 ) {
		spk_set_pixel( p_image, x1, y1, c );
		return;
	}
	w = abs( x2 - x1 );
	h = abs( y2 - y1 );
	vx = (x1 == x2) ? 0 : (x1 < x2) ? 1 : -1;
	vy = (y1 == y2) ? 0 : (y1 < y2) ? 1 : -1;
	n = 0;
	if( w > h ) {
		for( i = 0; i <= w; i++ ) {
			spk_set_pixel( p_image, x1, y1, c );
			x1 += vx;
			n += h;
			if( n >= w ) {
				n -= w;
				y1 += vy;
			}
		}
	}
	else {
		for( i = 0; i <= h; i++ ) {
			spk_set_pixel( p_image, x1, y1, c );
			y1 += vy;
			n += w;
			if( n >= h ) {
				n -= h;
				x1 += vx;
			}
		}
	}
}

// --------------------------------------------------------------------
static void inline _sort2( int *p1, int *p2 ) {
	int t;

	if( *p1 < *p2 ) return;
	t = *p1;
	*p1 = *p2;
	*p2 = t;
}

// --------------------------------------------------------------------
static int _sort_and_clip( int *p_x1, int *p_x2, int width ) {

	_sort2( p_x1, p_x2 );
	if( *p_x1 >= width && *p_x2 < 0 ) {
		return 0;
	}
	if( *p_x1 < 0 ) {
		*p_x1 = 0;
	}
	if( *p_x2 >= width ) {
		*p_x2 = width - 1;
	}
	return 1;
}

// --------------------------------------------------------------------
void spk_fill_rect( SPK_BACKBUFFER_T *p_image, int x1, int y1, int x2, int y2, uint8_t c ) {
	int w, y, pos;

	if( _sort_and_clip( &x1, &x2, p_image->width  ) ) return;
	if( _sort_and_clip( &y1, &y2, p_image->height ) ) return;
	//	drawing
	w = x2 - x1 + 1;
	pos = x1 + p_image->width * y1;
	for( y = y1; y <= y2; y++ ) {
		memset( p_image->image + pos, c, w );
		pos += p_image->width;
	}
}

// --------------------------------------------------------------------
int _clip( int *p_sx1, int *p_sx2, int *p_dx1, int src_width, int dest_width ) {
	int dx2;

	//	Handling of cases where the specified area of the transfer source extends beyond the source back buffer
	//	転送元の領域指定が、転送元のバックバッファーをはみ出す場合の処理 
	dx2 = *p_dx1 + (*p_sx2 - *p_sx1);
	if( *p_sx1 < 0 && *p_sx2 < 0 ) return 0;
	if( *p_sx1 >= src_width && *p_sx2 >= src_width ) return 0;
	if( *p_sx1 < 0 ) {
		*p_dx1 += -*p_sx1;
		*p_sx1 = 0;
	}
	if( *p_sx2 < 0 ) {
		dx2 += -*p_sx2;
		*p_sx2 = 0;
	}
	if( *p_sx1 >= src_width ) {
		*p_dx1 -= *p_sx1 - src_width;
	}
	if( *p_sx2 >= src_width ) {
		dx2 -= *p_sx2 - src_width;
	}
	//	Handling of cases where the destination back buffer is exceeded
	//	転送先バックバッファーをはみ出す場合の処理 
	if( *p_dx1 < 0 && dx2 < 0 ) return 0;
	if( *p_dx1 >= dest_width && dx2 >= dest_width ) return 0;
	if( *p_dx1 < 0 ) {
		*p_sx1 += -*p_dx1;
		*p_dx1 = 0;
	}
	if( dx2 < 0 ) {
		*p_sx2 += -dx2;
		dx2 = 0;
	}
	if( *p_dx1 >= dest_width ) {
		*p_sx1 -= *p_dx1 - dest_width;
	}
	if( dx2 >= dest_width ) {
		*p_sx2 -= dx2 - dest_width;
	}
	return 1;
}

// --------------------------------------------------------------------
void spk_copy( SPK_BACKBUFFER_T *p_src_image, int sx1, int sy1, int sx2, int sy2, SPK_BACKBUFFER_T *p_dest_image, int dx1, int dy1 ) {
	int x, y, vx, vy;
	uint8_t *p_src, *p_dest, d;
	uint8_t *p_src_st, *p_dest_st;

	//	clipping
	if( !_clip( &sx1, &sx2, &dx1, p_src_image->width , p_dest_image->width  ) ) return;
	if( !_clip( &sy1, &sy2, &dy1, p_src_image->height, p_dest_image->height ) ) return;

	vx			= (sx1 < sx2) ? 1 : -1;
	vy			= (sy1 < sy2) ? 1 : -1;
	p_src		= p_src_image->image  + (sx1 + sy1 * p_src_image->width );
	p_dest		= p_dest_image->image + (dx1 + dy1 * p_dest_image->width);
	p_src_st	= p_src;
	p_dest_st	= p_dest;
	for( y = sy1; ; y += vy ) {
		p_src	= p_src_st;
		p_dest	= p_dest_st;
		for( x = sx1; ; x += vx ) {
			d = *(p_src++);
			if( d ) {
				*p_dest = d;
			}
			p_dest++;
			if( x == sx2 ) break;
		}
		p_src_st	+= p_src_image->width * vy;
		p_dest_st	+= p_dest_image->width * vy;
		if( y == sy2 ) break;
	}
}

// --------------------------------------------------------------------
void spk_stretch_copy( SPK_BACKBUFFER_T *p_src_image, int sx1, int sy1, int sx2, int sy2, SPK_BACKBUFFER_T *p_dest_image, int dx1, int dy1, int dx2, int dy2 ) {
	int x, y, vx, vy, sx, sy, sw, dw, sh, dh, dx1d, dx2d, dy1d, dy2d;
	uint8_t d;

	if( (dx1 < 0) && (dx2 < 0) ) return;
	if( (dy1 < 0) && (dy2 < 0) ) return;
	if( (dx1 >= p_dest_image->width)  && (dx2 >= p_dest_image->width)  ) return;
	if( (dy1 >= p_dest_image->height) && (dy2 >= p_dest_image->height) ) return;
	vx = (dx1 < dx2) ? 1 : -1;
	vy = (dy1 < dy2) ? 1 : -1;
	sw = abs(sx2 - sx1) + 1;
	sh = abs(sy2 - sy1) + 1;
	dw = abs(dx2 - dx1) + 1;
	dh = abs(dy2 - dy1) + 1;
	dx1d = (dx1 < 0) ? 0 : (dx1 >= p_dest_image->width ) ? p_dest_image->width  - 1: dx1;
	dx2d = (dx2 < 0) ? 0 : (dx2 >= p_dest_image->width ) ? p_dest_image->width  - 1: dx2;
	dy1d = (dy1 < 0) ? 0 : (dy1 >= p_dest_image->height) ? p_dest_image->height - 1: dy1;
	dy2d = (dy2 < 0) ? 0 : (dy2 >= p_dest_image->height) ? p_dest_image->height - 1: dy2;
	for( y = dy1d; ; y += vy ) {
		sy = (y - dy1) * sh / dh + sy1;
		for( x = dx1d; ; x += vx ) {
			sx = (x - dx1) * sw / dw + sx1;
			d = spk_get_pixel( p_src_image, sx, sy );
			if( d ) {
				spk_set_pixel( p_dest_image, x, y, d );
			}
			if( x == dx2d ) break;
		}
		if( y == dy2d ) break;
	}
}

// --------------------------------------------------------------------
static int inline _linear( int sx1, int sx2, int dx, int dxs, int dw ) {

	return (sx2 - sx1) * (dx - dxs) / dw + sx1;
}

// --------------------------------------------------------------------
void spk_rotate_copy( SPK_BACKBUFFER_T *p_src_image, int sx1, int sy1, int sx2, int sy2, int sx3, int sy3, SPK_BACKBUFFER_T *p_dest_image, int dx1, int dy1, int dx2, int dy2 ) {
	int sx4, sy4, sx1y, sy1y, sx2y, sy2y, sx, sy, x, y, vx, vy, dw, dh, dx1d, dx2d, dy1d, dy2d;
	uint8_t d;

	vx = (dx1 < dx2) ? 1 : -1;
	vy = (dy1 < dy2) ? 1 : -1;
	dw = abs(dx2 - dx1) + 1;
	dh = abs(dy2 - dy1) + 1;
	dx1d = (dx1 < 0) ? 0 : (dx1 >= p_dest_image->width ) ? p_dest_image->width  - 1: dx1;
	dx2d = (dx2 < 0) ? 0 : (dx2 >= p_dest_image->width ) ? p_dest_image->width  - 1: dx2;
	dy1d = (dy1 < 0) ? 0 : (dy1 >= p_dest_image->height) ? p_dest_image->height - 1: dy1;
	dy2d = (dy2 < 0) ? 0 : (dy2 >= p_dest_image->height) ? p_dest_image->height - 1: dy2;
	sx4 = sx3 + sx2 - sx1;
	sy4 = sy3 + sy2 - sy1;
	for( y = dy1d; ; y += vy ) {
		sx1y = _linear( sx1, sx3, y, dy1, dh );
		sy1y = _linear( sy1, sy3, y, dy1, dh );
		sx2y = _linear( sx2, sx4, y, dy1, dh );
		sy2y = _linear( sy2, sy4, y, dy1, dh );
		for( x = dx1d; ; x += vx ) {
			sx = _linear( sx1y, sx2y, x, dx1, dw );
			sy = _linear( sy1y, sy2y, x, dx1, dw );
			d = spk_get_pixel( p_src_image, sx, sy );
			if( d ) {
				spk_set_pixel( p_dest_image, x, y, d );
			}
			if( x == dx2d ) break;
		}
		if( y == dy2d ) break;
	}
}

// --------------------------------------------------------------------
void spk_led( SPK_LED_T led, SPK_LED_ON_T led_on ) {

	AIOWriteGPIO( led_pin[ led & 3 ], led_on );
}

// --------------------------------------------------------------------
unsigned int spk_get_key_state( void ) {
	unsigned int result;

	result = 
		( AIOReadGPIO( SW_UP		) << 7 ) |
		( AIOReadGPIO( SW_LEFT		) << 6 ) |
		( AIOReadGPIO( SW_RIGHT		) << 5 ) |
		( AIOReadGPIO( SW_DOWN		) << 4 ) |
		( AIOReadGPIO( SW_BUTTON	) );
	return result;
}
