// --------------------------------------------------------------------
// Sharpikeebo game library DEMO
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
#include <stdlib.h>
#include <string.h>
#include "sharpikeebo_glib.h"
#include "game.h"

#define SHOT_NUM		16
#define PLAYER_SPEED	8

typedef struct {
	int x;
	int y;
} PLAYER_T;

typedef struct {
	int enable;
	int x;
	int y;
} SHOT_T;

typedef struct {
	int x1;
	int x2;
} BG_T;

// --------------------------------------------------------------------
static void player_move( SPK_BACKBUFFER_T *p_screen, PLAYER_T *p_player, SHOT_T *pp_shot ) {
	SPK_KEY_STATE_T k;
	static int shot_index = 0;

	k = spk_get_key_state();
	if( !pp_shot[shot_index].enable && (k & SPK_KEY_BUTTON) != 0 ) {
		pp_shot[shot_index].x = p_player->x;
		pp_shot[shot_index].y = p_player->y;
		pp_shot[shot_index].enable = 1;
		shot_index = (shot_index + 1) % SHOT_NUM;
	}
	if( (k & SPK_KEY_UP) != 0 ) {
		p_player->y = p_player->y - PLAYER_SPEED;
		if( p_player->y < 0 ) {
			p_player->y = 0;
		}
	}
	if( (k & SPK_KEY_DOWN) != 0 ) {
		p_player->y = p_player->y + PLAYER_SPEED;
		if( p_player->y > (240 - 32) ) {
			p_player->y = 240 - 32;
		}
	}
	if( (k & SPK_KEY_LEFT) != 0 ) {
		p_player->x = p_player->x - PLAYER_SPEED;
		if( p_player->x < 0 ) {
			p_player->x = 0;
		}
	}
	if( (k & SPK_KEY_RIGHT) != 0 ) {
		p_player->x = p_player->x + PLAYER_SPEED;
		if( p_player->x > (400 - 32 - 60) ) {
			p_player->x = 400 - 32 - 60;
		}
	}
	spk_copy( p_game, 0, 0, 31, 31, p_screen, p_player->x, p_player->y );
}

// --------------------------------------------------------------------
static void shot_move( SPK_BACKBUFFER_T *p_screen, SHOT_T *p_shot ) {

	if( !p_shot->enable ) {
		return;
	}
	p_shot->x = p_shot->x + 32;
	if( p_shot->x > 400 ) {
		p_shot->enable = 0;
		return;
	}
	spk_copy( p_game, 32, 0, 63, 31, p_screen, p_shot->x, p_shot->y );
}

// --------------------------------------------------------------------
static void back_ground( SPK_BACKBUFFER_T *p_screen, BG_T *p_bg, PLAYER_T *p_player ) {
	int y1, y2;

	p_bg->x1 = (p_bg->x1 + 1) % 800;
	y1 = 20 - p_player->y * 20 / 240 + 50;
	spk_copy( p_game, 0, 100, 799, 299, p_screen,     - p_bg->x1, y1 );
	spk_copy( p_game, 0, 100, 799, 299, p_screen, 800 - p_bg->x1, y1 );

	p_bg->x2 = (p_bg->x2 + 2) % 800;
	y2 = 40 - p_player->y * 40 / 240 + 60;
	spk_copy( p_game, 0, 300, 799, 599, p_screen,     - p_bg->x2, y2 );
	spk_copy( p_game, 0, 300, 799, 599, p_screen, 800 - p_bg->x2, y2 );
}

// --------------------------------------------------------------------
static void demo( void ) {
	SPK_BACKBUFFER_T *p_screen;
	PLAYER_T player;
	SHOT_T shot[SHOT_NUM];
	BG_T bg;
	int i;

	p_screen = spk_get_display();
	memset( shot, 0, sizeof(shot) );
	player.x = 200 - 16;
	player.y = 120 - 16;
	memset( &bg, 0, sizeof(bg) );

	// main loop
	for(;;) {
		spk_clear_buffer( p_screen, 0 );
		back_ground( p_screen, &bg, &player );
		player_move( p_screen, &player, shot );
		for( i = 0; i < SHOT_NUM; i++ ) {
			shot_move( p_screen, &shot[i] );
		}
		spk_display( p_screen );
	}
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {

	if( !spk_initialize() ) {
		printf( "ERROR: Failed spk_initialize()\n" );
		return 1;
	}

	demo();
	spk_terminate();
	return 0;
}
