// --------------------------------------------------------------------
// Sharpikeebo graphic library
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

#ifndef __SHARPIKEEBO_GLIB_H__
#define __SHARPIKEEBO_GLIB_H__

#include <stdint.h>

// --------------------------------------------------------------------
//	GLIB_BACKBUFFER_T
// --------------------------------------------------------------------
typedef struct {
	uint32_t	width;
	uint32_t	height;
	uint8_t		image[1];
} GLIB_BACKBUFFER_T;

// --------------------------------------------------------------------
//	glib_initialize()
//	input)
//		none
//	output)
//		0 ..... Failed.
//		!0 .... Success.
// --------------------------------------------------------------------
int glib_initialize( void );

// --------------------------------------------------------------------
//	glib_terminate()
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void glib_terminate( void );

// --------------------------------------------------------------------
//	glib_display()
//	input)
//		p_image .... 8bit image data (0...127: black, 128...255: white)
//	output)
//		none
//	comment)
//		Size must be 400x240.
// --------------------------------------------------------------------
void glib_display( const GLIB_BACKBUFFER_T *p_image );

// --------------------------------------------------------------------
//	glib_get_backbuffer()
//	input)
//		width ...... width of target backbuffer
//		height ..... height of target backbuffer
//	output)
//		NULL ....... failed
// --------------------------------------------------------------------
GLIB_BACKBUFFER_T *glib_get_backbuffer( uint32_t width, uint32_t height );

// --------------------------------------------------------------------
//	glib_get_display()
//	input)
//		none
//	output)
//		NULL ....... failed
// --------------------------------------------------------------------
GLIB_BACKBUFFER_T *glib_get_display( void );

// --------------------------------------------------------------------
//	glib_release_backbuffer()
//	input)
//		p_image .... target backbuffer pointer
//	output)
//		none
// --------------------------------------------------------------------
void glib_release_backbuffer( GLIB_BACKBUFFER_T *p_image );

// --------------------------------------------------------------------
//	glib_clear_buffer()
//	input)
//		p_image .... target backbuffer pointer
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void glib_clear_buffer( GLIB_BACKBUFFER_T *p_image, uint8_t c );

// --------------------------------------------------------------------
//	glib_pixel()
//	input)
//		p_image .... target backbuffer pointer
//		x .......... X position
//		y .......... Y position
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void glib_pixel( GLIB_BACKBUFFER_T *p_image, int x, int y, uint8_t c );

// --------------------------------------------------------------------
//	glib_line()
//	input)
//		p_image .... target backbuffer pointer
//		x1 ......... start X position
//		y1 ......... start Y position
//		x2 ......... end X position
//		y2 ......... end Y position
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void glib_line( GLIB_BACKBUFFER_T *p_image, int x1, int y1, int x2, int y2, uint8_t c );

// --------------------------------------------------------------------
//	glib_fill_rect()
//	input)
//		p_image .... target backbuffer pointer
//		x1 ......... start X position
//		y1 ......... start Y position
//		x2 ......... end X position
//		y2 ......... end Y position
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void glib_fill_rect( GLIB_BACKBUFFER_T *p_image, int x1, int y1, int x2, int y2, uint8_t c );

// --------------------------------------------------------------------
//	glib_led
//	input)
//		led ...... 0: Left-up, 1: Right-up, 2: Left-down, 3: Right-down
//		led_on ... 0: off, 1: on
//	output)
//		none
//	comment)
//		LED position
//			0 1
//			2 3
// --------------------------------------------------------------------
typedef enum {
	GLIB_LED_LU = 0,
	GLIB_LED_RU = 1,
	GLIB_LED_LD = 2,
	GLIB_LED_RD = 3,
} GLIB_LED_T;

typedef enum {
	GLIB_LED_OFF = 0,
	GLIB_LED_ON = 1,
} GLIB_LED_ON_T;

void glib_led( GLIB_LED_T led, GLIB_LED_ON_T led_on );

// --------------------------------------------------------------------
//	glib_get_key_state
//	input)
//		none
//	output)
//		bit7 ... 0: up off    , 1: up on
//		bit6 ... 0: left off  , 1: left on
//		bit5 ... 0: right off , 1: right on
//		bit4 ... 0: down off  , 1: down on
//		bit0 ... 0: button off, 1: button on
// --------------------------------------------------------------------
typedef enum {
	GLIB_KEY_UP		= 1 << 7,
	GLIB_KEY_LEFT	= 1 << 6,
	GLIB_KEY_RIGHT	= 1 << 5,
	GLIB_KEY_DOWN	= 1 << 4,
	GLIB_KEY_BUTTON	= 1 << 0,
} GLIB_KEY_STATE_T;

GLIB_KEY_STATE_T glib_get_key_state( void );

#endif
