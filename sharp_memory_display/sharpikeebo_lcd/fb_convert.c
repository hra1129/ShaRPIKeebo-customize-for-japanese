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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <armbianio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include "fb_convert.h"

static const char *s_fb				= "/dev/fb0";

static const int ref_width				= 400;
static const int ref_height				= 240;

static int hfb = 0;
static int ref_bits_per_pixel = 0;
static int ref_size = 0;

// --------------------------------------------------------------------
int fbc_initialize( void ) {
	struct fb_var_screeninfo si;

	hfb = open( s_fb, O_RDONLY );
	if( hfb == -1 ) {
		fprintf( stderr, "[ERROR] Cannot open framebuffer.\n" );
		return 0;	// Failed
	}
	ioctl( hfb, FBIOGET_VSCREENINFO, &si );
	if( si.bits_per_pixel != 32 && si.bits_per_pixel != 16 ) {
		fprintf( stderr, "[ERROR] Non-supported bit depths.\n" );
		return 0;	// Failed
	}
	ref_bits_per_pixel = si.bits_per_pixel;
	if( si.xres_virtual != ref_width || si.yres_virtual != ref_height ) {
		fprintf( stderr, "[ERROR] Non-supported size of framebuffer.\n" );
		return 0;	// Failed
	}
	ref_size = ref_width * ref_height * ref_bits_per_pixel / 8;
	return 1;	// Success
}

// --------------------------------------------------------------------
void fbc_terminate( void ) {
	close( hfb );
}

// --------------------------------------------------------------------
int fbc_get_size( int *p_width, int *p_height, int *p_bpp ) {

	if( p_width != NULL ) {
		*p_width = ref_width;
	}
	if( p_height != NULL ) {
		*p_height = ref_height;
	}
	if( p_bpp != NULL ) {
		*p_bpp = ref_bits_per_pixel;
	}
	return ref_size;
}

// --------------------------------------------------------------------
void fbc_capture( void *p_image ) {

	lseek( hfb, 0, SEEK_SET );
	read( hfb, p_image, ref_size );
}
