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

#include <unistd.h>
#include <armbianio.h>
#include "sharpikeebo_effects.h"
#include "sharp_memory_display_driver.h"

static const unsigned char splash_logo[] = {
#include "startup_logo.txt"
};

#define LED0			15		//	LEFT-UP
#define LED1			37		//	RIGHT-UP
#define LED2			35		//	LEFT-DOWN
#define LED3			33		//	RIGHT-DOWN

#define SW_LEFT			12		//	BUT1	SW102
#define SW_BUTTON		32		//	BUT2	SW101
#define SW_DOWN			36		//	BUT3	SW103
#define SW_RIGHT		38		//	BUT4	SW104
#define SW_UP			40		//	BUT5	SW105

static const int led_pin[] = {
	LED0, LED1, LED2, LED3
};

// --------------------------------------------------------------------
void spk_initialize( void ) {
	AIOInitBoard( "Raspberry Pi" );

	AIOAddGPIO( LED0		, GPIO_OUT );
	AIOAddGPIO( LED1		, GPIO_OUT );
	AIOAddGPIO( LED2		, GPIO_OUT );
	AIOAddGPIO( LED3		, GPIO_OUT );

	AIOAddGPIO( SW_UP		, GPIO_IN );
	AIOAddGPIO( SW_LEFT		, GPIO_IN );
	AIOAddGPIO( SW_RIGHT	, GPIO_IN );
	AIOAddGPIO( SW_DOWN		, GPIO_IN );
	AIOAddGPIO( SW_BUTTON	, GPIO_IN );

	AIOWriteGPIO( LED0		, SPK_LED_OFF );
	AIOWriteGPIO( LED1		, SPK_LED_OFF );
	AIOWriteGPIO( LED2		, SPK_LED_OFF );
	AIOWriteGPIO( LED3		, SPK_LED_OFF );
}

// --------------------------------------------------------------------
void spk_terminate( void ) {
}

// --------------------------------------------------------------------
void spk_splash( void ) {
	smdd_transfer_bitmap( splash_logo );
	usleep( 5000000 );
}

// --------------------------------------------------------------------
void spk_led( SPK_LED_T led, SPK_LED_ON_T led_on ) {

	AIOWriteGPIO( led_pin[ led & 3 ], led_on );
}

// --------------------------------------------------------------------
unsigned int spk_get_key_state( void ) {
	unsigned int result;

	result = 
		( AIOReadGPIO( SW_UP		) << 7 ) |
		( AIOReadGPIO( SW_LEFT		) << 6 ) |
		( AIOReadGPIO( SW_RIGHT		) << 5 ) |
		( AIOReadGPIO( SW_DOWN		) << 4 ) |
		( AIOReadGPIO( SW_BUTTON	) );
	return result;
}
