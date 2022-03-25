// --------------------------------------------------------------------
// Sharpikeebo graphic library DEMO
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
#include "sharpikeebo_glib.h"
#include "startup_logo.h"

// --------------------------------------------------------------------
static void demo( void ) {
	GLIB_BACKBUFFER_T *p_screen;
	int x1, y1, x2, y2, i, j;

	p_screen = glib_get_display();

	for( j = 0; j < 500; j++ ) {
		glib_clear_buffer( p_screen, 0 );
		glib_copy( p_startup_logo, 0, 0, 399, 239, p_screen, j - 400, 0 );
		glib_display( p_screen );
	}

	for( j = 0; j < 500; j++ ) {
		glib_clear_buffer( p_screen, 0 );
		for( i = 0; i < 50; i++ ) {
			x1 = rand() % 400;
			y1 = rand() % 240;
			x2 = rand() % 400;
			y2 = rand() % 240;
			glib_line( p_screen, x1, y1, x2, y2, 255 );
		}
		glib_display( p_screen );
	}

	for( j = 0; j < 500; j++ ) {
		glib_clear_buffer( p_screen, 0 );
		for( i = 0; i < 50; i++ ) {
			x1 = rand() % 400;
			y1 = rand() % 240;
			x2 = rand() % 400;
			y2 = rand() % 240;
			glib_fill_rect( p_screen, x1, y1, x2, y2, 255 );
		}
		glib_display( p_screen );
	}
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {

	if( !glib_initialize() ) {
		printf( "ERROR: Failed glib_initialize()\n" );
		return 1;
	}

	demo();
	glib_terminate();
	return 0;
}
