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
	uint32_t	clock;
} SCC_T;

#ifndef SAMPLE_RATE
#define SAMPLE_RATE		48000		//	Hz
#endif

#ifndef SCC_CLOCK
#define SCC_CLOCK		3579545		//	Hz
#endif

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
void scc_generate_wave( H_SCC_T hscc, int16_t *pwave, int samples ) {
}

// --------------------------------------------------------------------
void scc_write_register( H_SCC_T hscc, uint16_t address, uint8_t data ) {
}
