// --------------------------------------------------------------------
// Test of PSG
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

#include <psg_emulator.h>
#include <stdio.h>

int main( int argc, char *argv[] ) {
	H_PSG_T hpsg;
	static int16_t wave[2048];
	int i, j;

	hpsg = psg_initialize();

	psg_write_register( hpsg, 7, 0b10111110 );
	psg_write_register( hpsg, 0, 1 );
	psg_write_register( hpsg, 1, 0 );
	psg_write_register( hpsg, 6, 31 );
	psg_write_register( hpsg, 8, 16 );
	psg_write_register( hpsg, 11, 100 );
	psg_write_register( hpsg, 12, 10 );

	for( j = 0; j < 50; j++ ) {
		psg_write_register( hpsg, 13, 8 );
		psg_generate_wave( hpsg, wave, 1280 );
		for( i = 0; i < 1280; i++ ) {
			printf( "%d\n", (int) wave[i] );
		}
	}
	psg_terminate( hpsg );
	return 0;
}
