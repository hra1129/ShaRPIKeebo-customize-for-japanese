// --------------------------------------------------------------------
// PSG emulator
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
#include <unistd.h>
#include <psg_emulator.h>

typedef struct {
	int			tone_enable;
	int			noise_enable;
	int			envelope_enable;
	int			periodic_register;
	int			counter;
	int			volume;
	int			envelope_period;
	int			envelope_type;
	int			envelope_counter;
	int			envelope_state;
	int			tone;
	int			envelope;
	int			last_level;
} PSG_1CH_T;

typedef struct {
	uint32_t	samples;
	uint32_t	clock;
	int			clock_div32;
	uint8_t		registers[16];
	PSG_1CH_T	channel[3];
	uint32_t	noise_seed;
	int			noise_count;
	int			last_noise;
} PSG_T;

#ifndef SAMPLE_RATE
#define SAMPLE_RATE		48000		//	Hz
#endif

#ifndef PSG_CLOCK
#define PSG_CLOCK		3579545		//	Hz
#endif

static const int volume_table[] = {
	0x00, 	0x01, 	0x02, 	0x03,
	0x05, 	0x07, 	0x0B, 	0x0F,
	0x16, 	0x1F, 	0x2D, 	0x3F,
	0x5A, 	0x7F, 	0xB4, 	0xFF,
};

#define ENVELOPE_HOLD		0
#define ENVELOPE_ALTER		1
#define ENVELOPE_ATTACK		2
#define ENVELOPE_CONT		3
#define BIT( d, n )			(((d) >> (n)) & 1)

// --------------------------------------------------------------------
H_PSG_T psg_initialize( void ) {
	PSG_T *ppsg;

	ppsg = (PSG_T*) malloc( sizeof(PSG_T) );
	if( ppsg == NULL ) {
		return NULL;
	}

	memset( ppsg, 0, sizeof(PSG_T) );
	ppsg->noise_seed = 0x3FFFF;
	return ppsg;
}

// --------------------------------------------------------------------
void psg_terminate( H_PSG_T hpsg ) {

	free( hpsg );
}

// --------------------------------------------------------------------
static int _tone_generator( PSG_T *ppsg, PSG_1CH_T *pch, int noise ) {
	int index;

	if( (ppsg->clock_div32 & 15) != 0 ) {
		return pch->last_level;
	}

	if( pch->counter == 0 ) {
		pch->counter	= pch->periodic_register;
		pch->tone		= 1 - pch->tone;
	}
	else {
		pch->counter--;
	}

	if( ppsg->clock_div32 == 0 ) {
		if( pch->envelope_counter == 0 ) {
			pch->envelope_counter = pch->envelope_period;

			if( pch->envelope_state != 0 ) {
				if( (BIT(pch->envelope_state, 4) == 1) || (BIT(pch->envelope_type, ENVELOPE_HOLD) == 0 && BIT(pch->envelope_type, ENVELOPE_CONT) == 1) ) {
					pch->envelope_state = (pch->envelope_state - 1) & 31;
				}
			}
			else {
				if( BIT(pch->envelope_type, ENVELOPE_CONT) == 0 ) {
					pch->envelope = 0;
					pch->envelope_state = 0;
				}
				else {
					pch->envelope_state = 31;
				}
			}

			if( BIT(pch->envelope_state, 4) == 0 && BIT(pch->envelope_type, ENVELOPE_CONT) == 0 ) {
				pch->envelope = 0;
			}
			else if( BIT(pch->envelope_state, 4) == 1 || (BIT(pch->envelope_type, ENVELOPE_ALTER) ^ BIT(pch->envelope_type, ENVELOPE_HOLD)) == 0 ) {
				if( BIT(pch->envelope_state, ENVELOPE_ATTACK) == 1 ) {
					pch->envelope = (pch->envelope_state & 15) ^ 15;
				}
				else {
					pch->envelope = (pch->envelope_state & 15);
				}
			}
			else {
				if( BIT(pch->envelope_state, ENVELOPE_ATTACK) == 1 ) {
					pch->envelope = (pch->envelope_state & 15);
				}
				else {
					pch->envelope = (pch->envelope_state & 15) ^ 15;
				}
			}
		}
		else {
			pch->envelope_counter--;
		}
	}

	if( ((pch->tone_enable == 0 || pch->tone == 1) && (pch->noise_enable == 0 || noise == 1)) == 0 ) {
		pch->last_level = 0;
	}
	else {
		if( pch->envelope_enable ) {
			index = pch->envelope;
		}
		else {
			index = pch->volume;
		}
		pch->last_level = volume_table[ index ];
	}
	return pch->last_level;
}

// --------------------------------------------------------------------
static int inline _noise_generator( PSG_T *ppsg ) {

	if( ppsg->clock_div32 ) return ppsg->last_noise;

	if( ppsg->noise_count == 0 ) {
		ppsg->last_noise = BIT(ppsg->noise_seed,17);
		if( ppsg->noise_seed ) {
			ppsg->noise_seed = ( (ppsg->noise_seed << 1) | (BIT(ppsg->noise_seed,17) ^ BIT(ppsg->noise_seed,14)) ) & 0x3FFFF;
		}
		else {
			ppsg->noise_seed = (ppsg->noise_seed << 1) | 1;
		}
		ppsg->noise_count = (int) ppsg->registers[6];
	}
	else {
		ppsg->noise_count--;
	}
	return ppsg->last_noise;
}

// --------------------------------------------------------------------
void psg_generate_wave( H_PSG_T hpsg, uint8_t *pwave, int samples ) {
	int i, j, ch0, ch1, ch2, noise, next_clock, level;
	PSG_T *ppsg = (PSG_T*) hpsg;

	for( i = 0; i < samples; i++ ) {
		next_clock = ppsg->samples * PSG_CLOCK / SAMPLE_RATE;
		ppsg->samples++;

		level = 0;
		for( j = ppsg->clock; j < next_clock; j++ ) {
			//	1clock
			ppsg->clock_div32 = (ppsg->clock_div32 + 1) & 31;
			noise	= _noise_generator( ppsg );
			ch0		= _tone_generator( ppsg, &ppsg->channel[0], noise );
			ch1		= _tone_generator( ppsg, &ppsg->channel[1], noise );
			ch2		= _tone_generator( ppsg, &ppsg->channel[2], noise );
			level	+= (ch0 + ch1 + ch2) >> 2;
		}
		pwave[i] = (int8_t)( level / (next_clock - ppsg->clock) );
		ppsg->clock = next_clock;

		if( ppsg->samples >= SAMPLE_RATE ) {
			ppsg->clock		-= PSG_CLOCK;
			ppsg->samples	-= SAMPLE_RATE;
		}
	}
}

// --------------------------------------------------------------------
void psg_write_register( H_PSG_T hpsg, uint8_t address, uint8_t data ) {
	int psg_address;
	PSG_T *ppsg = (PSG_T*) hpsg;

	psg_address = address & 15;
	ppsg->registers[ psg_address ] = data;
	switch( psg_address ) {
	case 0:
	case 1:
		ppsg->registers[1]					= ppsg->registers[1] & 15;
		ppsg->channel[0].periodic_register	= (int)ppsg->registers[0] | (((int)ppsg->registers[1]) << 8);
		break;
	case 2:
	case 3:
		ppsg->registers[3]					= ppsg->registers[3] & 15;
		ppsg->channel[1].periodic_register	= (int)ppsg->registers[2] | (((int)ppsg->registers[3]) << 8);
		break;
	case 4:
	case 5:
		ppsg->registers[5]					= ppsg->registers[5] & 15;
		ppsg->channel[2].periodic_register	= (int)ppsg->registers[4] | (((int)ppsg->registers[5]) << 8);
		break;
	case 7:
		ppsg->channel[0].tone_enable		= ( (data &  1) == 0 );
		ppsg->channel[1].tone_enable		= ( (data &  2) == 0 );
		ppsg->channel[2].tone_enable		= ( (data &  4) == 0 );
		ppsg->channel[0].noise_enable		= ( (data &  8) == 0 );
		ppsg->channel[1].noise_enable		= ( (data & 16) == 0 );
		ppsg->channel[2].noise_enable		= ( (data & 32) == 0 );
		break;
	case 8:
		ppsg->channel[0].volume				= data & 15;
		ppsg->channel[0].envelope_enable	= (data >> 4) & 1;
		break;
	case 9:
		ppsg->channel[1].volume				= data & 15;
		ppsg->channel[1].envelope_enable	= (data >> 4) & 1;
		break;
	case 10:
		ppsg->channel[2].volume				= data & 15;
		ppsg->channel[2].envelope_enable	= (data >> 4) & 1;
		break;
	case 11:
	case 12:
		ppsg->channel[0].envelope_period	= (int)ppsg->registers[11] | (((int)ppsg->registers[12]) << 8);
		ppsg->channel[1].envelope_period	= ppsg->channel[0].envelope_period;
		ppsg->channel[2].envelope_period	= ppsg->channel[0].envelope_period;
		break;
	case 13:
		ppsg->channel[0].envelope_type		= data;
		ppsg->channel[0].envelope_state		= 31;
		ppsg->channel[0].envelope_counter	= ppsg->channel[0].envelope_period;
		ppsg->channel[1].envelope_type		= data;
		ppsg->channel[1].envelope_state		= 31;
		ppsg->channel[1].envelope_counter	= ppsg->channel[1].envelope_period;
		ppsg->channel[2].envelope_type		= data;
		ppsg->channel[2].envelope_state		= 31;
		ppsg->channel[2].envelope_counter	= ppsg->channel[2].envelope_period;
		break;
	default:
		break;
	}
}
