// --------------------------------------------------------------------
// Sharpikeebo game library
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

#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

//	pulseaudio
#include <pulse/error.h>
#include <pulse/simple.h>

#include <psg_emulator.h>
#include <scc_emulator.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
	// semun is defined into <sys/sem.h>. */
#else
union semun {
	int val;					// value for SETVAL
	struct semid_ds *buf;		// buffer for IPC_STAT, IPC_SET
	unsigned short int *array;	// array for GETALL, SETALL
	struct seminfo *__buf;		// buffer for IPC_INFO
};
#endif

#ifndef SAMPLE_RATE
#define SAMPLE_RATE		48000		//	Hz
#endif

#define SAMPLE_CHANNELS	2
#define SAMPLE_UNIT		( SAMPLE_RATE / 2 )
#define SAMPLE_SIZE		( SAMPLE_UNIT * SAMPLE_CHANNELS )
#define SAMPLE_BUFFER	( SAMPLE_RATE * SAMPLE_CHANNELS * 2 )

// --------------------------------------------------------------------
static int update_sem_id		= -1;	//	for SharpMemoryDisplay update lock semaphore
static pthread_t h_wave_thread;
static pthread_t h_timer_thread;		//	ÅöébíË 
static pa_simple *pa;
static volatile int exit_request = 0;
static struct sembuf lock_operations;
static struct sembuf unlock_operations;

static H_PSG_T hpsg;
static H_PSG_T hpsg_se;
static H_SCC_T hscc;

// --------------------------------------------------------------------
static void _sound_generator( int16_t *p_wave ) {
	static uint8_t wave[ SAMPLE_UNIT ];
	int i;

	memset( p_wave, 0, SAMPLE_SIZE * sizeof(int16_t) );

	psg_generate_wave( hpsg, wave, SAMPLE_UNIT );
	for( i = 0; i < SAMPLE_UNIT; i++ ) {
		p_wave[ (i << 1) + 0 ] = (int)wave[ i ] * 32;
		p_wave[ (i << 1) + 1 ] = (int)wave[ i ] * 32;
	}

	psg_generate_wave( hpsg_se, wave, SAMPLE_UNIT );
	for( i = 0; i < SAMPLE_UNIT; i++ ) {
		p_wave[ (i << 1) + 0 ] += (int)wave[ i ] * 32;
		p_wave[ (i << 1) + 1 ] += (int)wave[ i ] * 32;
	}

//	scc_generate_wave( hscc, wave, SAMPLE_UNIT );
//	for( i = 0; i < SAMPLE_UNIT; i++ ) {
//		p_wave[ (i << 1) + 0 ] = (int)wave[ i ] * 32;
//		p_wave[ (i << 1) + 1 ] = (int)wave[ i ] * 32;
//	}
}

// --------------------------------------------------------------------
static void *_wave_thread( void *p_arg ) {
	int16_t wave[ SAMPLE_SIZE ];
	int result, pa_error;

	for(;;) {
		//semop( update_sem_id, &lock_operations, 1 );
		if( exit_request ) break;
		_sound_generator( wave );
		result = pa_simple_write( pa, wave, sizeof(wave), &pa_error );
		if( result < 0 ) {
			pa_simple_free( pa );
			return NULL;
		}
	}
	pa_simple_free( pa );
	return NULL;
}

// --------------------------------------------------------------------
static void *_timer_thread( void *p_arg ) {		// ÅöébíË 
	for(;;) {
		semop( update_sem_id, &unlock_operations, 1 );
		if( exit_request ) break;
		usleep( 50 * 1000 - 100 );
	}
	return NULL;
}

// --------------------------------------------------------------------
int spk_sound_initialize( void ) {
	uint16_t wave[ SAMPLE_SIZE ];
	int pa_error;
	pa_sample_spec ss;
	union semun sem_arg;
	pa_buffer_attr buf_attr;

	hpsg	= psg_initialize();
	hpsg_se	= psg_initialize();
	hscc	= scc_initialize();
	if( hpsg == NULL || hpsg_se == NULL || hscc == NULL ) {
		return 0;
	}

	lock_operations.sem_num = 0;
	lock_operations.sem_op = -1;
	lock_operations.sem_flg = SEM_UNDO;
	unlock_operations.sem_num = 0;
	unlock_operations.sem_op = 1;
	unlock_operations.sem_flg = SEM_UNDO;

	update_sem_id = semget( IPC_PRIVATE, 1, S_IRUSR | S_IWUSR );
	if( update_sem_id == -1 ) {
		return 0;
	}
    sem_arg.val = 1;	//	unlocked
    semctl( update_sem_id, 0, SETVAL, sem_arg );

	ss.format			= PA_SAMPLE_S16LE;
	ss.rate				= SAMPLE_RATE;
	ss.channels			= SAMPLE_CHANNELS;

	buf_attr.minreq		= SAMPLE_SIZE * sizeof(int16_t);
	buf_attr.maxlength	= SAMPLE_BUFFER;
	buf_attr.prebuf		= buf_attr.minreq;
	buf_attr.tlength	= buf_attr.minreq;
	pa = pa_simple_new(NULL, "sharpikeebo_slib", PA_STREAM_PLAYBACK, NULL, "sharpikeebo_slib", &ss, NULL, &buf_attr, &pa_error);
	if (pa == NULL) {
		return 0;
	}

	memset( wave, 0, sizeof(wave) );
	pa_simple_write( pa, wave, sizeof(wave), &pa_error );

	pthread_create( &h_wave_thread, NULL, _wave_thread, NULL );
	pthread_create( &h_timer_thread, NULL, _timer_thread, NULL );	//	ÅöébíË 
	return 1;
}

// --------------------------------------------------------------------
void spk_sound_terminate( void ) {
	void *p_result;

	exit_request = 1;
	semop( update_sem_id, &lock_operations, 1 );
	pthread_join( h_wave_thread, &p_result );
	pthread_join( h_timer_thread, &p_result );

	scc_terminate( hscc );
	psg_terminate( hpsg_se );
	psg_terminate( hpsg );
}

// --------------------------------------------------------------------
H_PSG_T spk_get_psg_handle( void ) {

	return hpsg;
}

// --------------------------------------------------------------------
H_PSG_T spk_get_psg_se_handle( void ) {

	return hpsg_se;
}

// --------------------------------------------------------------------
H_SCC_T spk_get_scc_handle( void ) {

	return hscc;
}
