// --------------------------------------------------------------------
// sound demo
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
#include <unistd.h>
#include <sharpikeebo_slib.h>

#include <psg_emulator.h>

int main( int argc, char *argv[] ) {
	H_PSG_T hpsg;
	int i;
	
	spk_sound_initialize();
	hpsg = spk_get_psg_handle();

	psg_write_register( hpsg, 7, 0b10111110 );
/*
	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 8, 15 );		//	volume
	sleep( 1 );

	psg_write_register( hpsg, 0, 100 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );  		//	tone period (H)
	psg_write_register( hpsg, 8, 15 );		//	volume
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 2 );		//	tone period (H)
	psg_write_register( hpsg, 8, 15 );		//	volume
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 4 );		//	tone period (H)
	psg_write_register( hpsg, 8, 15 );		//	volume
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 0 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 20 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 0 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 4 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 8 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 10 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 11 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 12 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 13 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 14 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );

	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)
	psg_write_register( hpsg, 11, 0 );		//	envelope period (L)
	psg_write_register( hpsg, 12, 10 );		//	envelope period (H)
	psg_write_register( hpsg, 13, 15 );		//	envelope type
	psg_write_register( hpsg, 8, 16 );		//	use envelope
	sleep( 1 );
*/
	psg_write_register( hpsg, 7, 0b10110111 );
	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)

	for( i = 0; i < 31; i++ ) {
		psg_write_register( hpsg, 6, i );		//	noise
		psg_write_register( hpsg, 8, 15 );		//	use envelope
		sleep( 1 );
	}

	sleep( 3 );
	spk_sound_terminate();
	return 0;
}
