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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//	pulseaudio
#include <pulse/error.h>
#include <pulse/pulseaudio.h>
#include <pulse/mainloop.h>

#include <psg_emulator.h>
#include <scc_emulator.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE		48000		//	Hz
#endif

#define SAMPLE_CHANNELS	2

// --------------------------------------------------------------------
static pa_context *pa_ctx;
static pa_mainloop *pa_ml;
static pa_buffer_attr buf_attr;
static int underflows = 0;
static pa_sample_spec ss;

static pthread_t h_wave_thread;

static H_PSG_T hpsg;
static H_PSG_T hpsg_se;
static H_SCC_T hscc;

static int16_t wave[ SAMPLE_RATE * SAMPLE_CHANNELS * 2 ];
static int latency = 100;		// start latency in milli seconds

// --------------------------------------------------------------------
static void _sound_generator( int16_t *p_wave, int samples ) {
	static int16_t wave1[ SAMPLE_RATE ];
	static int16_t wave2[ SAMPLE_RATE ];
	static int16_t wave3[ SAMPLE_RATE ];
	int i;

	psg_generate_wave( hpsg,    wave1, samples );
	psg_generate_wave( hpsg_se, wave2, samples );
	scc_generate_wave( hscc,    wave3, samples );
	for( i = 0; i < samples; i++ ) {
		p_wave[ (i << 1) + 0 ] = (wave1[ i ] + wave2[ i ] + (wave3[ i ] << 1)) * 11;
		p_wave[ (i << 1) + 1 ] = (wave1[ i ] + wave2[ i ] + (wave3[ i ] << 1)) * 11;
	}
}

// --------------------------------------------------------------------
static void *_wave_thread( void *p_no_use ) {

	pa_mainloop_run( pa_ml, NULL );

	pa_context_disconnect( pa_ctx );
	pa_context_unref( pa_ctx );
	pa_ctx = NULL;
	return NULL;
}

// --------------------------------------------------------------------
static void stream_request_cb( pa_stream *s, size_t byte_length, void *p_no_use ) {
	//pa_usec_t usec;
	//int neg;
	int samples;

	byte_length &= ~3;
	if( byte_length > sizeof(wave) ) {
		byte_length = sizeof(wave);
	}
	samples = byte_length >> 2;
	//pa_stream_get_latency( s, &usec, &neg );
	//if( (sample_offset + (samples << 1)) > (sizeof(wave) >> 1) ) {
	//	sample_offset = 0;
	//}
	_sound_generator( wave, samples );
	pa_stream_write( s, wave, samples << 2, NULL, 0LL, PA_SEEK_RELATIVE );
	//sample_offset += samples << 1;
}

// --------------------------------------------------------------------
static void stream_underflow_cb(pa_stream *s, void *userdata) {
	// We increase the latency by 50% if we get 6 underflows and latency is under 2s
	// This is very useful for over the network playback that can't handle low latencies
	underflows++;
	if( underflows >= 6 && latency < 2000000 ) {
		latency = (latency * 3) / 2;
		buf_attr.maxlength = pa_usec_to_bytes( latency, &ss );
		buf_attr.tlength = pa_usec_to_bytes( latency, &ss );  
		pa_stream_set_buffer_attr( s, &buf_attr, NULL, NULL );
		underflows = 0;
	}
}

// --------------------------------------------------------------------
void pa_state_cb( pa_context *c, void *userdata ) {
	pa_context_state_t state;
	int *pa_ready = userdata;

	state = pa_context_get_state(c);
	switch  (state) {
	// These are just here for reference
	case PA_CONTEXT_UNCONNECTED:
	case PA_CONTEXT_CONNECTING:
	case PA_CONTEXT_AUTHORIZING:
	case PA_CONTEXT_SETTING_NAME:
	default:
		break;
	case PA_CONTEXT_FAILED:
	case PA_CONTEXT_TERMINATED:
		*pa_ready = 2;
		break;
	case PA_CONTEXT_READY:
		*pa_ready = 1;
		break;
	}
}

// --------------------------------------------------------------------
int spk_sound_initialize( void ) {
	pa_mainloop_api *pa_mlapi;
	pa_stream *playstream;
	int pa_ready = 0;
	int r;

	hpsg	= psg_initialize();
	hpsg_se	= psg_initialize();
	hscc	= scc_initialize();
	if( hpsg == NULL || hpsg_se == NULL || hscc == NULL ) {
		return 0;
	}

	pa_ml		= pa_mainloop_new();
	pa_mlapi	= pa_mainloop_get_api( pa_ml );
	pa_ctx		= pa_context_new( pa_mlapi, "sharpikeebo_slib" );
	pa_context_connect( pa_ctx, NULL, 0, NULL );

	pa_context_set_state_callback( pa_ctx, pa_state_cb, &pa_ready );
	while (pa_ready == 0) {
		pa_mainloop_iterate( pa_ml, 1, NULL );
	}
	if( pa_ready == 2 ) {
		//	case of PA_CONTEXT_FAILED or PA_CONTEXT_TERMINATED
		return 0;
	}
	//	case of PA_CONTEXT_READY
	ss.format			= PA_SAMPLE_S16LE;
	ss.rate				= SAMPLE_RATE;
	ss.channels			= SAMPLE_CHANNELS;
	playstream = pa_stream_new( pa_ctx, "sharpikeebo_slib Playback", &ss, NULL );
	if( playstream == NULL ) {
		return 0;
	}
	pa_stream_set_write_callback( playstream, stream_request_cb, NULL );
	pa_stream_set_underflow_callback( playstream, stream_underflow_cb, NULL );

	buf_attr.fragsize	= (uint32_t) -1;
	buf_attr.maxlength	= (uint32_t) -1;
	buf_attr.minreq		= (uint32_t) -1;
	buf_attr.prebuf		= (uint32_t) -1;
	buf_attr.tlength	= pa_usec_to_bytes( latency * 1000, &ss );
	r = pa_stream_connect_playback( playstream, NULL, &buf_attr,
			PA_STREAM_INTERPOLATE_TIMING | PA_STREAM_AUTO_TIMING_UPDATE | PA_STREAM_ADJUST_LATENCY,
			NULL, NULL );
	if( r < 0 ) {
		//	古い PulseAudio は PA_STREAM_ADJUST_LATENCY があるとうまくいかない場合があるので、外してリトライ.
		//	Old PulseAudio may not work with PA_STREAM_ADJUST_LATENCY, remove it and retry.
		r = pa_stream_connect_playback(playstream, NULL, &buf_attr,
			PA_STREAM_INTERPOLATE_TIMING | PA_STREAM_AUTO_TIMING_UPDATE, 
			NULL, NULL );
	}
	if( r < 0 ) {
		return 0;
	}

	pthread_create( &h_wave_thread, NULL, _wave_thread, NULL );
	return 1;
}

// --------------------------------------------------------------------
void spk_sound_terminate( void ) {
	void *p_result;

	if( pa_ml != NULL ) {
		pa_mainloop_quit( pa_ml, 0 );
		pthread_join( h_wave_thread, &p_result );
	}

	if( pa_ml != NULL ) {
		pa_mainloop_free( pa_ml );
		pa_ml = NULL;
	}

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
