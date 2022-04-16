// --------------------------------------------------------------------
// Test of SCC
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

#include <scc_emulator.h>
#include <stdio.h>
#include <stdint.h>

static const int8_t wave1[] = { 
	 127,  127,  127,  127,  127,  127,  127,  127, 
	 127,  127,  127,  127,  127,  127,  127,  127, 
	-128, -128, -128, -128, -128, -128, -128, -128, 
	-128, -128, -128, -128, -128, -128, -128, -128, 
};

// --------------------------------------------------------------------
static void set_wave( H_SCC_T hscc, int ch, const int8_t *p_wave ) {
	int i, address;

	address = 0xB800 + ch * 32;
	for( i = 0; i < 32; i++ ) {
		scc_write_register( hscc, address + i, p_wave[i] );
	}
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	H_SCC_T hscc;
	static int16_t wave[2048];
	int i, j;

	hscc = scc_initialize();

	set_wave( hscc, 0, wave1 );
	scc_write_register( hscc, 0xB8C0, 1 << 5 );
	scc_write_register( hscc, 0xB8A0, 100 );
	scc_write_register( hscc, 0xB8A1, 0 );
	scc_write_register( hscc, 0xB8AA, 15 );

	for( j = 0; j < 10; j++ ) {
		scc_generate_wave( hscc, wave, 1280 );
		for( i = 0; i < 1280; i++ ) {
			printf( "%d\n", (int) wave[i] );
		}
	}
	scc_terminate( hscc );
	return 0;
}
