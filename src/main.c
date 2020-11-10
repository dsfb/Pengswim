/*
 *  pengswim:  A game of swimming and eating
 *  Copyright (C) 2002  Michael J. Edwards and OneArmedMan LLC
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "gamedefs.h"
#include "grafix.h"
#include "sound.h"
#include "gameloop.h"

/*int active_fish = MAX_FISH;*/

int main()
{
	int begin,quit,done,scores;
	char *end_string;
	Uint8* keystate;
	
	srand ( time(NULL) );

	/* init SDL and error check */
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		printf("Unable to initiailize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	if (TTF_Init() != 0) {
		printf("Unable to initiailize TrueType Font engine: %s\n", TTF_GetError());
		return 1;
	}

	/*make sure SDL_Quit is fired at exit*/
	atexit(TTF_Quit);
	atexit(SDL_Quit);
	
	SDL_EnableKeyRepeat(0,30);
	
	if (init_grafix() == 1) return 1;
	
	if (init_sounds() == 1) return 1;
	
	begin = 0;
	quit = 0;
	done = 0;
	scores = 0;
	

	do {
		if (player_state!=PLAYER_CONTINUE) {
			score = 0;
			stop_track();
		}		

		if (!begin) {
			draw_ocean();
			draw_sky();
			draw_title();
			SDL_Flip(screen);
		} else {
			if (player_state==PLAYER_WIN) {
				end_string = "You won!  Play again? (y/n)";
			} else if (player_state==PLAYER_LOSE) {
				end_string = "You lost!  Play again? (y/n)";
			} else if (player_state==PLAYER_QUIT) {
				end_string = "You quit! Play again? (y/n)";
			} else if (player_state==PLAYER_CONTINUE) {
				end_string = "Continue to next level? (y/n)";
			}
			draw_text(end_string,screen->w/2,screen->h/2);
			SDL_Flip(screen);
		}
		
		while (!begin) {
			draw_title();
			SDL_Flip(screen);
			quit = 0;
			done = 0;
			play_track(TRACK_GAME);
			SDL_PumpEvents();
			keystate = SDL_GetKeyState(NULL);
			if (keystate[SDLK_RETURN]) {
				begin = 1;
			} else if (keystate[SDLK_h]) {
				scores = 1;
			} else if (keystate[SDLK_q] | keystate[SDLK_ESCAPE]) {
				begin = 1;
				quit = 1;
				done = 1;
			}
			while(scores) {
				scores = display_scores();
			}
		}
		
		start_time = SDL_GetTicks();

		while(!done) {
			play_track(TRACK_GAME);
			done = gameloop();
		}
		
		SDL_PumpEvents();

		keystate = SDL_GetKeyState(NULL);

		if (keystate[SDLK_n]|keystate[SDLK_q]|keystate[SDLK_ESCAPE]) {
			begin = 0;
		}

		if (keystate[SDLK_y]) {
			done = 0;
			quit = 0;
		}
	} while (!quit);
	
	kill_sounds();
	kill_grafix();
	
	return 0;
		
}
