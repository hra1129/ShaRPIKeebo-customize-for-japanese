// --------------------------------------------------------------------
// sound demo (SCC)
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
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <sharpikeebo_slib.h>

#include <scc_emulator.h>

static const char *s_tone_name[] = { "C(L)", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C(H)" };
static const int8_t wave1[] = { 
	 127,  127,  127,  127,  127,  127,  127,  127, 
	 127,  127,  127,  127,  127,  127,  127,  127, 
	-128, -128, -128, -128, -128, -128, -128, -128, 
	-128, -128, -128, -128, -128, -128, -128, -128, 
};

static const int8_t wave2[] = { 
	0x7F, 0x77, 0x6F, 0x67, 0x5F, 0x57, 0x4F, 0x47, 
	0x3F, 0x37, 0x2F, 0x27, 0x1F, 0x17, 0x0F, 0x07, 
	0xFF, 0xF7, 0xEF, 0xE7, 0xDF, 0xD7, 0xCF, 0xC7, 
	0xBF, 0xB7, 0xAF, 0xA7, 0x9F, 0x97, 0x8F, 0x87, 
};

static const int8_t wave3[] = { 
	0x7F, 0x7F, 0x4F, 0x4F, 0x1F, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static const int8_t wave4[] = { 
	0x7F, 0x7F, 0x4F, 0x4F, 0x6F, 0x6F, 0x1F, 0x1F, 
	0x7F, 0x7F, 0x4F, 0x4F, 0x6F, 0x6F, 0x1F, 0x1F, 
	0x7F, 0x7F, 0x4F, 0x4F, 0x6F, 0x6F, 0x1F, 0x1F, 
	0x7F, 0x7F, 0x4F, 0x4F, 0x2F, 0x2F, 0x00, 0x00, 
};

static const int8_t wave5[] = { 
	0x7F, 0x7F, 0x4F, 0x4F, 0x1F, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7F, 0x7F, 0x4F, 0x4F, 0x1F, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

static int tone_table[13];

// --------------------------------------------------------------------
static void makeup_tone_table( void ) {
	const double clock = 3579545.;			//	Hz
	double ratio, freq, tone_freq, cycles;
	int i;

	ratio	= pow( 2., 1./12. );
	freq	= 440 / pow( ratio, 9. );		//	frequency of C

	for( i = 0; i < 13; i++ ) {
		tone_freq		= freq * pow( ratio, (double) i );
		cycles			= clock / (tone_freq * 16);
		tone_table[i]	= (int) (cycles + 0.5) - 1;
	}
}

// --------------------------------------------------------------------
static void set_wave( H_SCC_T hscc, int ch, const int8_t *p_wave ) {
	int i, address;

	address = 0xB800 + ch * 32;
	for( i = 0; i < 32; i++ ) {
		scc_write_register( hscc, address + i, p_wave[i] );
	}
}

// --------------------------------------------------------------------
static void tone_test( H_SCC_T hscc, int ch, int tone, int time ) {

	printf( "Tone %s.\n", s_tone_name[ tone ] );

	scc_write_register( hscc, 0xB8A0 + ch * 2, tone_table[ tone ] & 255 );		//	tone period (L)
	scc_write_register( hscc, 0xB8A1 + ch * 2, tone_table[ tone ] >> 8 );		//	tone period (H)
	scc_write_register( hscc, 0xB8AA + ch, 15 );		//	volume
	sleep( time );
}

// --------------------------------------------------------------------
static void tone_test2( H_SCC_T hscc, int tone1, int tone2, int tone3, int time ) {

	printf( "Tone %s-%s-%s.\n", s_tone_name[ tone1 ], s_tone_name[ tone2 ], s_tone_name[ tone3 ] );

	scc_write_register( hscc, 0xB8A0, tone_table[ tone1 ] & 255 );		//	tone period (L)
	scc_write_register( hscc, 0xB8A1, tone_table[ tone1 ] >> 8 );		//	tone period (H)
	scc_write_register( hscc, 0xB8A2, tone_table[ tone2 ] & 255 );		//	tone period (L)
	scc_write_register( hscc, 0xB8A3, tone_table[ tone2 ] >> 8 );		//	tone period (H)
	scc_write_register( hscc, 0xB8A4, tone_table[ tone3 ] & 255 );		//	tone period (L)
	scc_write_register( hscc, 0xB8A5, tone_table[ tone3 ] >> 8 );		//	tone period (H)
	scc_write_register( hscc, 0xB8AA, 15 );		//	volume
	scc_write_register( hscc, 0xB8AB, 15 );		//	volume
	scc_write_register( hscc, 0xB8AC, 15 );		//	volume
	sleep( time );
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	H_SCC_T hscc;
	int i;
	
	spk_sound_initialize();
	hscc = spk_get_scc_handle();

	makeup_tone_table();

	set_wave( hscc, 0, wave1 );
	set_wave( hscc, 1, wave2 );
	set_wave( hscc, 2, wave3 );
	set_wave( hscc, 3, wave4 );
	set_wave( hscc, 4, wave5 );
	scc_write_register( hscc, 0xB8C0, 1 << 5 );
	for( i = 0; i < 5; i++ ) {
		printf( "Ch.%d\n", i + 1 );
		tone_test( hscc, i,  0, 1 );
		tone_test( hscc, i,  2, 1 );
		tone_test( hscc, i,  4, 1 );
		tone_test( hscc, i,  5, 1 );
		tone_test( hscc, i,  7, 1 );
		tone_test( hscc, i,  9, 1 );
		tone_test( hscc, i, 11, 1 );
		tone_test( hscc, i, 12, 1 );
		printf( "Stop.\n" );
		scc_write_register( hscc, 0xB8AA + i, 0 );	//	stop tone
		sleep( 1 );
	}

	tone_test2( hscc,  0, 4, 7, 1 );
	tone_test2( hscc,  2, 6, 9, 1 );
	printf( "Stop.\n" );
	scc_write_register( hscc, 0xB8AA, 0 );	//	stop tone
	scc_write_register( hscc, 0xB8AB, 0 );	//	stop tone
	scc_write_register( hscc, 0xB8AC, 0 );	//	stop tone
	sleep( 3 );
	spk_sound_terminate();
	return 0;
}
