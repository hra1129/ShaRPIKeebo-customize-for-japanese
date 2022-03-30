// --------------------------------------------------------------------
// Sharpikeebo game library DEMO
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
#include "usa.h"

#define USA_NUM		40

// --------------------------------------------------------------------
static void demo( void ) {
	SPK_BACKBUFFER_T *p_screen;
	int x1, y1, x2, y2, i, j;
	int usa_x[USA_NUM], usa_y[USA_NUM], usa_vx[USA_NUM], usa_vy[USA_NUM];

	p_screen = spk_get_display();

	for( j = 0; j < 100; j++ ) {
		spk_clear_buffer( p_screen, 0 );
		x1 = (j - 50) * 4;
		y1 = j;
		x2 = j * 16;
		y2 = j * 3;
		spk_stretch_copy( p_startup_logo, 0, 0, 399, 239, p_screen, x1, y1, x2, y2 );
		spk_display( p_screen );
	}

	for( i = 0; i < USA_NUM; i++ ) {
		usa_x[i] = (rand() % 400) << 8;
		usa_y[i] = (rand() % 240) << 8;
		usa_vx[i] = (rand() % 4097) - 2048;
		usa_vy[i] = (rand() % 4097) - 2048;
		if( (usa_vx[i] | usa_vy[i]) == 0 ) {
			usa_vx[i] = 256;
			usa_vy[i] = 256;
		}
	}

	for( j = 0; j < 500; j++ ) {
		spk_clear_buffer( p_screen, 0 );
		spk_copy( p_startup_logo, 0, 0, 399, 239, p_screen, j - 400, 0 );
		for( i = 0; i < USA_NUM; i++ ) {
			usa_x[i] += usa_vx[i];
			if( usa_vx[i] < 0 && usa_x[i] < 0 ) {
				usa_x[i] = 0;
				usa_vx[i] = -usa_vx[i];
			}
			else if( usa_vx[i] > 0 && usa_x[i] >= (400 << 8) ) {
				usa_x[i] = 400 << 8;
				usa_vx[i] = -usa_vx[i];
			}
			usa_y[i] += usa_vy[i];
			if( usa_vy[i] < 0 && usa_y[i] < 0 ) {
				usa_y[i] = 0;
				usa_vy[i] = -usa_vy[i];
			}
			else if( usa_vy[i] > 0 && usa_y[i] >= (400 << 8) ) {
				usa_y[i] = 400 << 8;
				usa_vy[i] = -usa_vy[i];
			}
			spk_copy( p_usa, 0, 0, 63, 66, p_screen, usa_x[i] >> 8, usa_y[i] >> 8 );
		}
		spk_display( p_screen );
	}

	for( j = 0; j < 500; j++ ) {
		spk_clear_buffer( p_screen, 0 );
		for( i = 0; i < 50; i++ ) {
			x1 = rand() % 400;
			y1 = rand() % 240;
			x2 = rand() % 400;
			y2 = rand() % 240;
			spk_line( p_screen, x1, y1, x2, y2, 255 );
		}
		spk_display( p_screen );
	}

	for( j = 0; j < 500; j++ ) {
		spk_clear_buffer( p_screen, 0 );
		for( i = 0; i < 50; i++ ) {
			x1 = rand() % 400;
			y1 = rand() % 240;
			x2 = rand() % 400;
			y2 = rand() % 240;
			spk_fill_rect( p_screen, x1, y1, x2, y2, 255 );
		}
		spk_display( p_screen );
	}
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {

	if( !spk_initialize() ) {
		printf( "ERROR: Failed spk_initialize()\n" );
		return 1;
	}

	demo();
	spk_terminate();
	return 0;
}
