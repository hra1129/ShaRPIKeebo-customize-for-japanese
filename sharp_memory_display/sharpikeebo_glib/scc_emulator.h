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

#ifndef __SCC_EMULATOR_H__
#define __SCC_EMULATOR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void *H_SCC_T;

// --------------------------------------------------------------------
//	scc_initialize
//	input)
//		none
//	output)
//		0 ...... Failed (Not enough memory.)
//		!0 ..... H_SCC_T instance.
// --------------------------------------------------------------------
H_SCC_T scc_initialize( void );

// --------------------------------------------------------------------
//	scc_terminate
//	input)
//		hscc ... H_SCC_T instance
//	output)
//		none
// --------------------------------------------------------------------
void scc_terminate( H_SCC_T hscc );

// --------------------------------------------------------------------
//	scc_generate_wave
//	input)
//		hscc ......... H_SCC_T instance
//		pwave ........ Wave memory address
//		samples ...... Samples of Wave memory
//	output)
//		none
//	comment)
//		An 8-bit signed monaural signal is written to pwave.
//		samples indicates the size of the pwave and is equal to the number of BYTEs.
// --------------------------------------------------------------------
void scc_generate_wave( H_SCC_T hscc, int16_t *pwave, int samples );

// --------------------------------------------------------------------
//	scc_write_register
//	input)
//		hpsg ......... H_SCC_T instance
//		address ...... SCC register address
//		data ......... Write data
//	output)
//		none
// --------------------------------------------------------------------
void scc_write_register( H_SCC_T hscc, uint16_t address, uint8_t data );

#ifdef __cplusplus
}
#endif

#endif
