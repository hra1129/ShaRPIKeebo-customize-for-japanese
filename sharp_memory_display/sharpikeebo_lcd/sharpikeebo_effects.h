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

#ifndef __SHARPIKEEBO_EFFECTS_H__
#define __SHARPIKEEBO_EFFECTS_H__

// --------------------------------------------------------------------
//	spk_initialize
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void spk_initialize( void );

// --------------------------------------------------------------------
//	spk_terminate
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void spk_terminate( void );

// --------------------------------------------------------------------
//	spk_splash
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void spk_splash( void );

// --------------------------------------------------------------------
//	spk_led
//	input)
//		led ...... 0: Left-up, 1: Right-up, 2: Left-down, 3: Right-down
//		led_on ... 0: off, 1: on
//	output)
//		none
//	comment)
//		LED position
//			0 1
//			2 3
// --------------------------------------------------------------------
typedef enum {
	SPK_LED_LU = 0,
	SPK_LED_RU = 1,
	SPK_LED_LD = 2,
	SPK_LED_RD = 3,
} SPK_LED_T;

typedef enum {
	SPK_LED_OFF = 0,
	SPK_LED_ON = 1,
} SPK_LED_ON_T;

void spk_led( SPK_LED_T led, SPK_LED_ON_T led_on );

// --------------------------------------------------------------------
//	spk_get_key_state
//	input)
//		none
//	output)
//		bit7 ... 0: up off    , 1: up on
//		bit6 ... 0: left off  , 1: left on
//		bit5 ... 0: right off , 1: right on
//		bit4 ... 0: down off  , 1: down on
//		bit0 ... 0: button off, 1: button on
// --------------------------------------------------------------------
typedef enum {
	SPK_KEY_UP		= 1 << 7,
	SPK_KEY_LEFT	= 1 << 6,
	SPK_KEY_RIGHT	= 1 << 5,
	SPK_KEY_DOWN	= 1 << 4,
	SPK_KEY_BUTTON	= 1 << 0,
} SPK_KEY_STATE_T;

SPK_KEY_STATE_T spk_get_key_state( void );

#endif
