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

#ifndef __FB_CONVERT_H__
#define __FB_CONVERT_H__

// --------------------------------------------------------------------
//	fbc_initialize
//	input)
//		none
//	output)
//		0 .... Failed.
//		!0 ... Success
// --------------------------------------------------------------------
int fbc_initialize( void );

// --------------------------------------------------------------------
//	fbc_terminate
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void fbc_terminate( void );

// --------------------------------------------------------------------
//	fbc_get_byte_size
//	input)
//		none
//	output)
//		size of framebuffer (bytes).
// --------------------------------------------------------------------
int fbc_get_size( int *p_width, int *p_height, int *p_bpp );


// --------------------------------------------------------------------
//	fbc_capture
//	input)
//		p_image ... Area for storing frame buffer contents.
//					400x240, 32bpp (
//	output)
//		*p_image .. result
// --------------------------------------------------------------------
void fbc_capture( void *p_image );

#endif
