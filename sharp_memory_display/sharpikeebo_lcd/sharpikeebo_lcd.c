// ------------------------------------------------------------------------
//  Framebuffer transfer service for sharp memory display
// ========================================================================
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
// ------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <armbianio.h>

#include "sharp_memory_display_driver.h"
#include "sharpikeebo_effects.h"
#include "fb_convert.h"

#define LED0			15		//	LEFT-UP
#define LED1			37		//	RIGHT-UP
#define LED2			35		//	LEFT-DOWN
#define LED3			33		//	RIGHT-DOWN
#define LED_ON			1
#define LED_OFF			0

#define CS				16
#define DISPON			18
#define EXTCOMIN		22

#define	INPUT			0
#define	OUTPUT			1

#define SPEED			4000000

//static int hspi;
static volatile int is_active = 0;

// --------------------------------------------------------------------
static void shutdown_handler( int signal ) {
	is_active = 0;
}

// --------------------------------------------------------------------
static int init_sharp_memory_display( void ) {
	AIOAddGPIO( LED0		, GPIO_OUT );
	AIOAddGPIO( LED1		, GPIO_OUT );
	AIOAddGPIO( LED2		, GPIO_OUT );
	AIOAddGPIO( LED3		, GPIO_OUT );

	AIOWriteGPIO( LED0		, LED_OFF );
	AIOWriteGPIO( LED1		, LED_OFF );
	AIOWriteGPIO( LED2		, LED_OFF );
	AIOWriteGPIO( LED3		, LED_OFF );
	return 1;
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	int size, c, i;
	struct sigaction sa;
	uint32_t *p_capture;
	int width, height;
	unsigned char *p_bitmap;
	int do_splash = 1;

	printf( "framebuffer transfer\n" );
	printf( "===========================\n" );
	printf( "2022/Mar/12nd t.hara\n" );

	sa.sa_handler = shutdown_handler;
	sigemptyset( &sa.sa_mask );
	sa.sa_flags = 0;
	sigaction( SIGINT, &sa, NULL );
	is_active = 1;

	if( !AIOInitBoard( "Raspberry Pi" ) ) {
		fprintf( stderr, "[ERROR] Not supported.\n" );
		return 1;
	}

	if( !fbc_initialize() ) {
		return 2;
	}

	init_sharp_memory_display();

	if( !smdd_initialize() ) {
		return 3;
	}

	// analyze the command-line options
	for( i = 1; i < argc; i++ ) {
		if( strcmp( argv[i], "-nosplash" ) == 0 ) {
			do_splash = 0;
		}
		else if( strcmp( argv[i], "-invert" ) == 0 ) {
			smdd_set_invert( 1 );
		}
		else if( strcmp( argv[i], "-thresold" ) == 0 && (i + 1) < argc ) {
			i++;
			smdd_set_threshold( atoi( argv[i] ) );
		}
		else {
			fprintf( stderr, "[WARNING] Unknown command line option %s.\n", argv[i] );
		}
	}

	if( do_splash ) {
		spk_splash();
	}

	size = fbc_get_size( &width, &height, NULL );
	p_capture = (uint32_t*) malloc( size );
	if( p_capture == NULL ) {
		fprintf( stderr, "[ERROR] Not enough memory.\n" );
		return 4;
	}

	p_bitmap = (unsigned char*) malloc( width * height / 8 );
	if( p_bitmap == NULL ) {
		fprintf( stderr, "[ERROR] Not enough memory.\n" );
		return 5;
	}

	c = 0;
	while( is_active ) {
		fbc_capture( p_capture );
		smdd_convert_image( p_capture, p_bitmap, c );
		smdd_transfer_bitmap( p_bitmap );
		c = 1 - c;
	}
	smdd_terminate();
	fbc_terminate();
	return 0;
}