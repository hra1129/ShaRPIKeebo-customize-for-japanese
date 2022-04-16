// --------------------------------------------------------------------
// SCC emulator
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

#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <scc_emulator.h>

typedef struct {
	int			tone_enable;
	int			periodic_register;
	int			counter;
	int			volume;
	int			sample_pos;
	int			last_level;
	int			wave[32];
} SCC_1CH_T;

typedef struct {
	uint32_t	samples;
	uint32_t	clock;
	int			counter_reset_mode;
	SCC_1CH_T	channel[5];
} SCC_T;

#ifndef SAMPLE_RATE
#define SAMPLE_RATE		48000		//	Hz
#endif

#ifndef SCC_CLOCK
#define SCC_CLOCK		3579545		//	Hz
#endif

#define BIT( d, n )			(((d) >> (n)) & 1)

// --------------------------------------------------------------------
H_SCC_T scc_initialize( void ) {
	SCC_T *pscc;

	pscc = (SCC_T*) malloc( sizeof(SCC_T) );
	if( pscc == NULL ) {
		return NULL;
	}

	memset( pscc, 0, sizeof(SCC_T) );
	return (H_SCC_T) pscc;
}

// --------------------------------------------------------------------
void scc_terminate( H_SCC_T hscc ) {

	free( hscc );
}

// --------------------------------------------------------------------
static int _tone_generator( SCC_T *pscc, SCC_1CH_T *pch, int increment ) {
	int shift, level;
	
	pch->counter += increment;
	if( pch->counter >= pch->periodic_register ) {
		shift				= pch->counter / (pch->periodic_register + 1);
		pch->sample_pos		= (pch->sample_pos + shift) & 31;
		level				= (pch->volume * pch->wave[ pch->sample_pos ]) >> 4;
		pch->last_level		= level;
		pch->counter		-= shift * (pch->periodic_register + 1);
	}
	else {
		level				= pch->last_level;
	}
	return level;
}

// --------------------------------------------------------------------
void scc_generate_wave( H_SCC_T hscc, int16_t *pwave, int samples ) {
	int i, next_clock, diff_clock, level;
	SCC_T *pscc = (SCC_T*) hscc;

	for( i = 0; i < samples; i++ ) {
		next_clock = (int)( (int64_t)pscc->samples * SCC_CLOCK / SAMPLE_RATE );
		diff_clock = next_clock - pscc->clock;
		pscc->samples++;

		level	= _tone_generator( pscc, &pscc->channel[0], diff_clock )
				+ _tone_generator( pscc, &pscc->channel[1], diff_clock )
				+ _tone_generator( pscc, &pscc->channel[2], diff_clock )
				+ _tone_generator( pscc, &pscc->channel[3], diff_clock )
				+ _tone_generator( pscc, &pscc->channel[4], diff_clock );
		pwave[i] = (int16_t) level;
		pscc->clock = next_clock;

		if( pscc->samples >= SAMPLE_RATE ) {
			pscc->clock		-= SCC_CLOCK;
			pscc->samples	-= SAMPLE_RATE;
		}
	}
}

// --------------------------------------------------------------------
void scc_write_register( H_SCC_T hscc, uint16_t address, uint8_t data ) {
	int ch;
	SCC_T *pscc = (SCC_T*) hscc;

	if( address < 0xB800 || address > 0xBFFF ) {
		return;
	}
	address -= 0xB800;
	if( address < 0x00A0 ) {
		//	wave memory
		ch = address >> 5;
		pscc->channel[ch].wave[address & 31] = (int8_t)data;
	}
	else if( (address & 0xFE) == 0xFE ) {
		//	mode register2
	}
	else if( (address & 0xC0) == 0xC0 ) {
		//	mode register1
		pscc->counter_reset_mode = BIT( data, 5 );
	}
	else if( (address & 0xAF) < 0xAA ) {
		//	frequency
		ch = (address >> 1) & 7;
		if( (address & 1) == 0 ) {
			pscc->channel[ch].periodic_register = (pscc->channel[ch].periodic_register & ~0xFF) | (int)data;
		}
		else {
			pscc->channel[ch].periodic_register = (pscc->channel[ch].periodic_register & 0xFF) | ((int)data << 8);
		}
		if( pscc->counter_reset_mode ) {
			pscc->channel[ch].counter = 0;
			pscc->channel[ch].sample_pos = 0;
		}
	}
	else if( (address & 0xAF) < 0xAF ) {
		//	volume
		ch = ((address & 0xAF) - 0xAA) & 7;
		pscc->channel[ch].volume = data & 15;
	}
	else { 	//	if( (address & 0xAF) == 0xAF ) {
		pscc->channel[0].tone_enable	= BIT( data, 0 );
		pscc->channel[1].tone_enable	= BIT( data, 1 );
		pscc->channel[2].tone_enable	= BIT( data, 2 );
		pscc->channel[3].tone_enable	= BIT( data, 3 );
		pscc->channel[4].tone_enable	= BIT( data, 4 );
	}
}
