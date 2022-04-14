// --------------------------------------------------------------------
// Sharp Memory Display Driver
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

#ifndef __SHARP_MEMORY_DISPLAY_DRIVER_H__
#define __SHARP_MEMORY_DISPLAY_DRIVER_H__

#include <stdint.h>

// --------------------------------------------------------------------
//	smdd_initialize()
//	input)
//		none
//	output)
//		0 ..... Failed.
//		!0 .... Success.
// --------------------------------------------------------------------
int smdd_initialize( void );

// --------------------------------------------------------------------
//	smdd_terminate()
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void smdd_terminate( void );

// --------------------------------------------------------------------
//	smdd_display()
//	input)
//		switch_on .... 0: display off, 1: display on
//	output)
//		none
// --------------------------------------------------------------------
void smdd_display( int switch_on );

// --------------------------------------------------------------------
//	smdd_transfer_bitmap()
//	input)
//		p_image .... 1bpp image data
//	output)
//		none
//	comment)
//		The contents of p_image are destroyed.
// --------------------------------------------------------------------
void smdd_transfer_bitmap( unsigned char *p_image );

// --------------------------------------------------------------------
//	smdd_convert_image()
//	input)
//		p_src ....... 32bpp image data
//		p_dest ...... 1bpp image data address
//		c ........... 0: even frame, 1: odd frame
//		zoom_x ...... X
//		zoom_y ...... Y
//		zoom_level .. 0: normal, 1: x2 zoom, 2: x4 zoom, 3: x8 zoom
//	output)
//		*p_dest .. 1bpp image data
// --------------------------------------------------------------------
void smdd_convert_image( const void *p_src, unsigned char *p_dest, int c, int zoom_x, int zoom_y, int zoom_level );

// --------------------------------------------------------------------
//	smdd_set_invert()
//	input)
//		inv ...... 0: not invert, 1: invert
//	output)
//		none
// --------------------------------------------------------------------
void smdd_set_invert( int inv );

// --------------------------------------------------------------------
//	smdd_set_blink()
//	input)
//		blink ...... 0: not blink, 1: blink
//	output)
//		none
// --------------------------------------------------------------------
void smdd_set_blink( int blink );

// --------------------------------------------------------------------
//	smdd_set_threshold()
//	input)
//		thres .... threshold for binary conversion.
//	output)
//		none
// --------------------------------------------------------------------
void smdd_set_threshold( int thres );

#endif
