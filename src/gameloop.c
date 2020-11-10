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
 
/*control loop for the game*/

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gamedefs.h"
#include "gameloop.h"
#include "grafix.h"
#include "sound.h"
#include "particle.h"

static int levels[MAX_LEVELS][MAX_LINES] = {
	{1048575,524289,524289,525825,525825,524289,524289,524289},
	{1048575,917511,524289,524289,524289,524289,525825,528129,3840,3840},
	{1048575,524289,524289,0,1536,0,524289,524289},
	{1048575,528129,528129,528129,525825,524289,524289,524289},
	{1048575,0,0,786435,786435,524289,524289,524289},
	{1048575,32,32,32,16416,16384,16384,16384},
	{1048575,524289,524289,524289,524289,524289,524289,524289},
	{1048575,1015839,1015839,1015839,0,0,0,0},
	{1048575,1536,1536},
	{1048575}
};



/*default high scores*/
struct hiscore_s {
	char * player_name;
	int player_score;
} hiscore[] = {
	{"Tux",50},
	{"Tux",45},
	{"Tux",40},
	{"Tux",35},
	{"Tux",30},
	{"Tux",25},
	{"Tux",20},
	{"Tux",15},
	{"Tux",10},
	{"Tux",5}
};

void init_ice(int start_fish)
{
	int i,j,level;
	
	level = start_fish / 2 - 1;
	
	for (i=0;i<MAX_ICE;i++) {
		ice[i].x = 0;
		ice[i].y = 0;
		ice[i].birth = 0;
	}
	
	for (i=0;i<MAX_LINES;i++) {
		for (j=0;j<MAX_LINE_WIDTH;j++) {
			if ((int)pow(2,(double)j) & levels[level][i]) {
				ice[j + i * MAX_LINE_WIDTH].x = j * ice_surf->w + ice_surf->w/2;
				ice[j + i * MAX_LINE_WIDTH].y = screen->h - i * ice_surf->h - ice_surf->h/2;
				ice[j + i * MAX_LINE_WIDTH].birth = SDL_GetTicks();
			}
		}
	}
	
	/*for (i=0;i<=(int)floor(screen->h/(ice_surf->h*2))+2;i++) {
		for (j=0;j<=(int)floor(screen->w/ice_surf->w);j++) {
			if (j + i * (int)floor(screen->w/ice_surf->w) >= MAX_ICE) break;
			if (i==0) {
				ice[j + i * ((int)floor(screen->w/ice_surf->w)+1)].x = j * ice_surf->w;
				ice[j + i * ((int)floor(screen->w/ice_surf->w)+1)].y = screen->h - i * ice_surf->h;
				ice[j + i * ((int)floor(screen->w/ice_surf->w)+1)].birth = SDL_GetTicks();
			} else if (j==0 | j== (int)floor(screen->w/ice_surf->w) 
			| j == (int)(floor(screen->w/(ice_surf->w*2))) & i < (int)(floor(screen->h/(ice_surf->h*2))-2)) {
				ice[j + i * ((int)floor(screen->w/ice_surf->w)+1)].x = j * ice_surf->w;
				ice[j + i * ((int)floor(screen->w/ice_surf->w)+1)].y = screen->h - i * ice_surf->h;
				ice[j + i * ((int)floor(screen->w/ice_surf->w)+1)].birth = SDL_GetTicks();
			}
		}
	}*/
}

void init_penguins()
{
	int i;
	
	for (i=0; i<NUM_PENGUINS; i++) {
		penguins[0].x = 0;
		penguins[0].y = screen->h/4;
		penguins[0].v = MAX_SPEED * 2;
		penguins[0].a = 0;
		penguins[0].dx = MAX_SPEED * 2;
		penguins[0].dy = 0;
		penguins[0].ang = 0;
		penguins[0].worldx = penguins[0].x;
		penguins[0].worldy = penguins[0].y;
		penguins[0].charging = 0;
		penguins[0].collision = 0;
	}
}

void init_fish(int start_fish)
{
	int i;
	
	for (i=0; i<MAX_FISH; i++) {
		fish[i].x = rand() % screen->w;
		fish[i].y = rand() % screen->h/2 + screen->h/2;
		fish[i].v = 8;
		fish[i].dx = ((rand() % 3) - 1) * 2 * fish[i].v;
		fish[i].dy = (rand() % 3) - 1 * fish[i].v;
		fish[i].a = 0;
		fish[i].active = 0;
		fish[i].ang = atan2(-fish[i].dy, fish[i].dx) * 180 / PI;
		fish[i].worldx = fish[i].x;
		fish[i].worldy = fish[i].y;
		fish[i].collision = 0;
		if (check_ice(fish[i].x,fish[i].y,64.0)) i--;
	}
}

static void move_fish(double time_scale) 
{
	int i;
	int ice_x, ice_y;
	
	for (i = 0; i<active_fish; i++) {
		
		if (rand() % (int)floor(50 * time_scale) == 0) {
			fish[i].ang += rand() % 90 - 45;
			fish[i].dx = fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale;
			fish[i].dy = fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale;
		}		

		/*bounce 'em*/
		/*if (check_ice(fish[i].x + (int)(fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale)
			,fish[i].y + (int)(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale),64.0)
			& fish[i].collision == 0) {
			fish[i].ang += 180;
			if (fish[i].ang < 0) fish[i].ang += 360;
			if (fish[i].ang >= 360) fish[i].ang -= 360;
			fish[i].collision = 1;
		} else if (check_ice(fish[i].x + (int)(fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale),fish[i].y,64.0)
			& fish[i].collision == 0) {
			fish[i].ang = atan2(-fish[i].dy,-fish[i].dx) * 180 / PI;
			fish[i].collision = 1;
		} else if (check_ice(fish[i].x,fish[i].y + (int)(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale),64.0)
			& fish[i].collision == 0) {
			fish[i].ang = atan2(-fish[i].dy,-fish[i].dx) * 180 / PI;
			fish[i].collision = 1;
		} else {
			fish[i].collision = 0;
		}*/

		if ((check_ice2(fish[i].x + (int)(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale),fish[i].y,48.0, &ice_x, &ice_y))
			| check_ice2(fish[i].x,fish[i].y + (int)(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale),48.0, &ice_x, &ice_y)) {
			fish[i].ang = atan2(-(fish[i].y - ice_y),(fish[i].x - ice_x))*180/PI;
			fish[i].dx = fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale;
			fish[i].dy = fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale;				
			if (!fish[i].collision) {
				//printf("Fish collision\n");
				fish[i].collision = 1;
				fish[i].x = ice_x + floor(48 * cos(fish[i].ang*PI/180.0));
				fish[i].y = ice_y + floor(48 * -sin(fish[i].ang*PI/180.0));
				/*fish[i].ang += 180;
				if (fish[i].ang < 0) fish[i].ang += 360;
				if (fish[i].ang >= 360) fish[i].ang -= 360;
				*/
				/*if (check_ice(fish[i].x + (int)(fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale),fish[i].y,48.0)) {
					fish[i].x -= (int)(fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale);
					fish[i].dx *= -1;
					//fish[i].ang = atan2(-fish[i].dy,-fish[i].dx) * 180 / PI;
				} else if (check_ice(fish[i].x,fish[i].y + (int)(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale),48.0)) {
					fish[i].y -= (int)(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale);
					fish[i].dy *= -1;
					//fish[i].ang = atan2(fish[i].dy,fish[i].dx) * 180 / PI;
				}*/
			} else {
				//delete_fish(i);
				fish[i].x += floor(fish[i].dx * time_scale);
				fish[i].y += floor(fish[i].dy * time_scale);
			}
		} else {
			/*move the fish */
			fish[i].x += fish[i].dx * time_scale;
			fish[i].y += fish[i].dy * time_scale;
			fish[i].collision = 0;
		}

		/*if (check_ice(fish[i].x,fish[i].y,32.0)){
			fish[i].x += 0;
			fish[i].y -= 32;
		}*/

		if ((fish[i].y + fish[i].dy) < screen->h/2 | (fish[i].y + fish[i].dy) > screen->h | rand() % 75 == 0) {
			//fish[i].ang = atan2(fish[i].dy,fish[i].dx) * 180 / PI;
			fish[i].dy *= -1;
		}
		
		if ((fish[i].x + fish[i].dx) < 0 | (fish[i].x + fish[i].dx) > screen->w | rand() % 75 == 0) {
			//fish[i].ang = atan2(-fish[i].dy,-fish[i].dx) * 180 / PI;
			fish[i].dx *= -1;
		}
		
		/*look out! a penguin!*/
		if (rand() % 3 == 0) {
			if (check_penguins(fish[i].x,fish[i].y,96.0)){
				fish[i].ang = atan2(-(fish[i].y - penguins[0].y),(fish[i].x - penguins[0].x))*180/PI;
				CreateBubble(fish[i].x,fish[i].y);
				fish[i].dx = fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale;
				fish[i].dy = fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale;
			}
		}
 	   
		if (fish[i].y < screen->h/2) {
			fish[i].y = screen->h/2;
		}

		if (fish[i].y > screen->h - 32) {
			fish[i].y = screen->h - 32;
		}
		
		if (fish[i].x < 32) {
			fish[i].x = 32;
		}
		
		if (fish[i].x > screen->w - 32) {
			fish[i].x = screen->w - 32;
		}

		//fish[i].dx = floor(fish[i].v * cos(fish[i].ang*PI/180.0) * time_scale);
		//fish[i].dy = floor(fish[i].v * -sin(fish[i].ang*PI/180.0) * time_scale);

		fish[i].ang = atan2(-fish[i].dy,fish[i].dx) * 180 / PI;

		if (fish[i].dy * fish[i].dy + fish[i].dx * fish[i].dx > fish[i].v * fish[i].v) {
			fish[i].dy = -sin(fish[i].ang*PI/180.0) * fish[i].v;
			fish[i].dx = cos(fish[i].ang*PI/180.0) * fish[i].v;
		}

		
	}
}

void delete_fish(int index)
{
    /* Replace the particle with the one at the end
       of the list, and shorten the list. */
    fish[index] = fish[active_fish-1];
    active_fish--;
    score++;
}

static void check_fish(int x, int y, int dist)
{
	int i;
	
	for (i=0;i<active_fish;i++) {
		if (pow(abs(x-fish[i].x),2) + pow(abs(y-fish[i].y),2) < pow(dist,2)) {
			delete_fish(i);
			play_sound(SOUND_GULP);
			if (active_fish == 0) {
				play_sound(SOUND_BURP);
			}
		}
	}
}

void add_seabird()
{
	int i;
	
	if (active_seabirds < MAX_SEABIRDS) {
		i = active_seabirds;
		seabirds[i].x = (rand() % 2) * screen->w;
		seabirds[i].y = screen->h/4 - 100 + rand() % 200;
		seabirds[i].v = 4;
		seabirds[i].a = 0;
		seabirds[i].ang = seabirds[i].x * 180 / screen->w;
		seabirds[i].worldx = seabirds[i].x;
		seabirds[i].worldy = seabirds[i].y;
		seabirds[i].birth = SDL_GetTicks();
		active_seabirds++;
	}
}

static void delete_seabird(int index)
{
    /* Replace the particle with the one at the end
       of the list, and shorten the list. */
    seabirds[index] = seabirds[active_seabirds-1];
    active_seabirds--;
}

static void move_seabirds(double time_scale) 
{
	int i;
	
	for (i = 0; i<active_seabirds; i++) {
		
		/*seabirds make only one move across in their life.  How sad for them.*/
		seabirds[i].x += floor(seabirds[i].v * cos(seabirds[i].ang*PI/180.0) * time_scale);

		seabirds[i].dx = floor(seabirds[i].v * cos(seabirds[i].ang*PI/180.0) * time_scale);

		if (seabirds[i].x > screen->w | seabirds[i].x < 0) {
			delete_seabird(i);
		}
	}
}

static void move_penguins(double time_scale, int peng_control)
{
	int i, turn;
	
	/*move the penguin by motion vector*/
	turn = 0;

	if (penguins[0].y > screen->h/2) {
		if (penguins[0].charging > 0) {
			penguins[0].v = MAX_SPEED * 2;
		}
		
		/*collision detection*/
		if (check_ice(penguins[0].x + (int)(penguins[0].v * cos(penguins[0].ang*PI/180.0) * time_scale)
			,penguins[0].y + (int)(penguins[0].v * -sin(penguins[0].ang*PI/180.0) * time_scale),40.0)) {
			if (!penguins[0].collision) {
				penguins[0].collision = 1;
				if (check_ice(penguins[0].x + (int)(penguins[0].v * cos(penguins[0].ang*PI/180.0) * time_scale),penguins[0].y,40.0)) {
					penguins[0].x -= (int)(penguins[0].v * cos(penguins[0].ang*PI/180.0) * time_scale);
					penguins[0].ang = atan2(-penguins[0].dy,-penguins[0].dx) * 180 / PI;
					penguins[0].v *= .8;
				} else if (check_ice(penguins[0].x,penguins[0].y + (int)(penguins[0].v * -sin(penguins[0].ang*PI/180.0) * time_scale),40.0)) {
					penguins[0].y -= (int)(penguins[0].v * -sin(penguins[0].ang*PI/180.0) * time_scale);
					penguins[0].ang = atan2(penguins[0].dy,penguins[0].dx) * 180 / PI;
					penguins[0].v *= .8;
				}
			}
		} else {
			/*move the penguin */
			penguins[0].x += (int)(penguins[0].v * cos(penguins[0].ang*PI/180.0) * time_scale);
			penguins[0].y += (int)(penguins[0].v * -sin(penguins[0].ang*PI/180.0) * time_scale);
			penguins[0].collision = 0;
		}
		
		/* if the penguin gets stuck in something, throw it out to the inside */
		if (check_ice(penguins[0].x,penguins[0].y,32.0)){
			penguins[0].x -= (int)2*(penguins[0].v * cos(penguins[0].ang*PI/180.0) * time_scale);
			penguins[0].y -= (int)2*(penguins[0].v * -sin(penguins[0].ang*PI/180.0) * time_scale);
		}
		
		/*calculate horiz. and vert. velocity*/
		penguins[0].dx = (int)(penguins[0].v * cos(penguins[0].ang*PI/180.0) * time_scale);
		penguins[0].dy = (int)(penguins[0].v * -sin(penguins[0].ang*PI/180.0) * time_scale);
				
		if (peng_control & PENG_RIGHT) {
			turn = -ANGLE_RATE;
		} 
		
		if (peng_control & PENG_LEFT) {
			turn = ANGLE_RATE;
		} 
	
		if (peng_control & PENG_UP) {
			penguins[0].a = MAX_ACCEL;
		} else if (peng_control & PENG_DOWN) {
			penguins[0].a = MAX_ACCEL * -5;
		} else {
			penguins[0].a = 0;
		}
		
		if ((peng_control & PENG_CHARGE) && penguins[0].charging == 0) {
			penguins[0].charging = CHARGE_TIME;
		} 
		
		penguins[0].charging -= (int) 10 * time_scale;
		
		if (penguins[0].charging < 0) {
			penguins[0].charging = 0;
		}
			
		penguins[0].v = penguins[0].v * exp(-1 * DRAG * time_scale) + penguins[0].a * time_scale;

		if (penguins[0].v < 0) {
			penguins[0].v = 0;
		}

		if (penguins[0].v > MAX_SPEED & penguins[0].charging == 0) {
			penguins[0].v = MAX_SPEED;
		}

		/*penguins[0].a += (-penguins[0].v * DRAG);*/

	} else {
		/*penguins[0].x += (int) (penguins[0].dx) * time_scale;
		penguins[0].y += (int) (penguins[0].dy) * time_scale;*/
		/*penguins[0].dx = (int) (penguins[0].v) * cos(penguins[0].ang*PI/180.0) * time_scale;
		*/
		penguins[0].dy += GRAVITY * time_scale;
		if (check_ice(penguins[0].x + (int)(penguins[0].dx)
			,penguins[0].y + (int)(penguins[0].dy),40.0)) {
			if (!penguins[0].collision) {
				penguins[0].collision = 1;
				if (check_ice(penguins[0].x + (int)penguins[0].dx,penguins[0].y,40.0)) {
					penguins[0].x -= (int)penguins[0].dx;
					penguins[0].dx *= -1;
					penguins[0].v *= .8;
				} else if (check_ice(penguins[0].x,penguins[0].y + (int)(penguins[0].dy),40.0)) {
					penguins[0].y -= (int)penguins[0].dy;
					penguins[0].dy *= -1;
					penguins[0].v *= .8;
				}
			}
		} else {
			penguins[0].x += (int)penguins[0].dx;
			penguins[0].y += (int)penguins[0].dy;
			penguins[0].collision = 0;
		}
		penguins[0].ang = atan2(-penguins[0].dy,penguins[0].dx) * 180 / PI;
		/*penguins[0].ang = atan(-penguins[0].dy/penguins[0].dx) * 180 / PI 
		+ (abs(penguins[0].dx) - penguins[0].dx) * 90 / penguins[0].dx; */
		penguins[0].a = 0;
	}			
		
    penguins[0].ang += turn * time_scale;
	    
    if (penguins[0].ang < 0) penguins[0].ang += 360;
    if (penguins[0].ang >= 360) penguins[0].ang -= 360;

	/*bounce 'em */
	/*if (penguins[0].y < 0) {
		penguins[0].ang = atan2(penguins[0].dy,penguins[0].dx) * 180 / PI;
		penguins[0].y = 0;
	}
	if (penguins[0].y > screen->h) {
		penguins[0].ang = atan2(penguins[0].dy,penguins[0].dx) * 180 / PI;
		penguins[0].y = screen->h;
	}
	if (penguins[0].x < 0) {
		penguins[0].ang = atan2(-penguins[0].dy,-penguins[0].dx) * 180 / PI;
		penguins[0].x = 0;
	}
	if (penguins[0].x > screen->w) {
		penguins[0].ang = atan2(-penguins[0].dy,-penguins[0].dx) * 180 / PI;
		penguins[0].x = screen->w;
	} */
 	
	/* wrap 'em */
	
	if (penguins[0].x > screen->w - 1) {
		penguins[0].x = 0;
	}
		
	if (penguins[0].x < 0) {
		penguins[0].x = screen->w - 1;
	}

	if (penguins[0].y <0) {
		penguins[0].dy = 0;
		penguins[0].y = 0;
	}
		
	if (penguins[0].y > screen->h - 1) {
		penguins[0].dy = 0;
		penguins[0].y = screen->h - 1;		
	}	
	
}

int check_ice(int x, int y, float dist)
{
	int i;
	
	for (i=0;i<MAX_ICE;i++) {
		if (pow(abs(x-ice[i].x),2) + pow(abs(y-ice[i].y),2) < pow(dist,2) & ice[i].birth > 0) {
			return 1;
		}
	}
	return 0;
}

int check_ice2(int x, int y, float dist, int * ice_x, int * ice_y)
{
	int i;
	
	for (i=0;i<MAX_ICE;i++) {
		if (pow(abs(x-ice[i].x),2) + pow(abs(y-ice[i].y),2) < pow(dist,2) & ice[i].birth > 0) {
			*ice_x = ice[i].x;
			*ice_y = ice[i].y;
			return 1;
		}
	}
	return 0;
}

int check_penguins(int x, int y, float dist)
{
	int i;
	
	for (i=0;i<NUM_PENGUINS;i++) {
		if (pow(abs(x-penguins[i].x),2) + pow(abs(y-penguins[i].y),2) < pow(dist,2)) {
			return 1;
		}
	}
	return 0;
}

int gameloop() 
{
	SDL_Rect src, dest;
	SDL_Event event;
	int frames, delay, bubble_x, bubble_y, i,j;
	double time_scale;
	Uint8 peng_control;
	Uint8* keystate;
	Uint32 start_ticks,end_ticks;
	char* time_string;
	int decimal,sign,time_left;
	int fps;

	frames = 0;
	peng_control = 0;
	delay = 0;
	start_ticks = SDL_GetTicks();
	end_ticks = start_ticks + TICK_DELAY;
	time_scale = 1.0;
	
	if (player_state != PLAYER_CONTINUE) {
		start_fish = 2;
	}

	active_fish = start_fish;
	start_fish += 2;

	init_ice(active_fish);
	init_penguins();
	init_fish(active_fish);
	
	for  (;;) {
		draw_ocean();
		draw_sky();
		
		if (rand()%250 == 0) {
			add_seabird();
		} 
		draw_seabirds(active_seabirds,time_scale);
		
		draw_penguins(penguins[0].ang);
		check_fish(penguins[0].x,penguins[0].y,20);
		draw_fish(active_fish);
		draw_ice();
				
		
		/*
		fps = floor(1000/(end_ticks - start_ticks));
		time_string = fcvt((double)fps, 0, &decimal, &sign);
		draw_text(time_string,5,5);
		
		time_string = fcvt((double)frames, 0, &decimal, &sign);
		draw_text(time_string,5,30);

		time_string = fcvt((double)peng_control, 0, &decimal, &sign);
		draw_text(time_string,5,55);

		time_string = fcvt((double)penguins[0].ang, 0, &decimal, &sign);
		draw_text(time_string,5,80);

		time_string = fcvt((double)penguins[0].v, 0, &decimal, &sign);
		draw_text(time_string,5,105);
		*/
		
		time_left = MAX_TIME - (SDL_GetTicks() - start_time)/1000;
		draw_score(score, time_left);
		
		DrawParticles(screen,0,0);
		UpdateBubbles(time_scale);
		UpdateSplashes(time_scale);

		/* swap the back buffer */
		SDL_Flip(screen);


		SDL_PumpEvents();
		
		keystate = SDL_GetKeyState(NULL);
		
		peng_control = 0;
		
		if (keystate[SDLK_q] || keystate[SDLK_ESCAPE]) {
			player_state = PLAYER_QUIT;
			return 1;
		}
		
		if (keystate[SDLK_UP]) {
			peng_control = peng_control | PENG_UP;
			/*peng_control = peng_control & !PENG_DOWN;*/
		}

		if (keystate[SDLK_DOWN]) {
			peng_control = peng_control | PENG_DOWN;
			/*peng_control = peng_control & !PENG_UP;*/
		}

		if (keystate[SDLK_LEFT]) {
			peng_control = peng_control | PENG_LEFT;
			/*peng_control = peng_control & !PENG_RIGHT;*/
		}

		if (keystate[SDLK_RIGHT]) {
			peng_control = peng_control | PENG_RIGHT;
			/*peng_control = peng_control & !PENG_LEFT;*/
		}

		if (keystate[SDLK_SPACE]) {
			peng_control = PENG_CHARGE;
			/*peng_control = peng_control & !PENG_LEFT;*/
		}

		/*Move the penguins for the next frame */
		move_penguins(time_scale,peng_control);
		move_fish(time_scale);
		move_seabirds(time_scale);
		
		bubble_x = (int)32 * cos(floor(penguins[0].ang /15)*15*PI/180.0) + penguins[0].x;
		bubble_y = (int)32 * -sin(floor(penguins[0].ang /15)*15*PI/180.0) + penguins[0].y - 16;
		
		if (rand()%10 == 0) {
			CreateBubble(bubble_x,bubble_y);
		}

		if (penguins[0].y > screen->h/2 & penguins[0].y - penguins[0].dy + GRAVITY * time_scale <= screen->h/2 |
			penguins[0].y <= screen->h/2 & penguins[0].y - penguins[0].dy > screen->h/2) {
			play_sound(SOUND_SPLASH);
			CreateSplash(penguins[0].x,penguins[0].y);
			for (i=0;i<5;i++) {
				bubble_x = penguins[0].x + rand()%32 - 16;
				bubble_y = screen->h/2 + rand()%32;
				CreateBubble(bubble_x,bubble_y);
			}
		}

		if (penguins[0].charging > 0) {
			for (i=0;i<3;i++) {
				bubble_x = (int)32 * -cos(floor(penguins[0].ang /15)*15*PI/180.0) + penguins[0].x + rand()%32;
				bubble_y = (int)32 * sin(floor(penguins[0].ang /15)*15*PI/180.0) + penguins[0].y + rand()%32;
				CreateBubble(bubble_x,bubble_y);
			}
		}

		if (penguins[0].v > 4) {
			bubble_x = (int)32 * -cos(floor(penguins[0].ang /15)*15*PI/180.0) + penguins[0].x + rand()%32;
			bubble_y = (int)32 * sin(floor(penguins[0].ang /15)*15*PI/180.0) + penguins[0].y + rand()%32;
			CreateBubble(bubble_x,bubble_y);
		}

		time_scale = (double)((end_ticks-start_ticks) / (double)TICK_DELAY);

		start_ticks = end_ticks;
		end_ticks = SDL_GetTicks();

		frames++;
		/*don't want to tie up the processor
		SDL_Delay(5);*/
		
		if (SDL_GetTicks() - start_time > MAX_TIME * 1000) {
			player_state = PLAYER_LOSE;
			return 1;
		}
		
		if (active_fish == 0) { 
			if (start_fish <= MAX_FISH) {
				player_state = PLAYER_CONTINUE;
				return 1;
			} else {
				player_state = PLAYER_WIN;
				return 1;			
			}
		}
	}
	return 1;
}

int display_scores() {
	Uint8* keystate;
	
	for (;;) {
		SDL_PumpEvents();
		
		keystate = SDL_GetKeyState(NULL);
				
		draw_ocean();
		draw_sky();

		draw_scores(hiscore);
		
		SDL_Flip(screen);
		
		if (keystate[SDLK_q] || keystate[SDLK_ESCAPE] || keystate[SDLK_RETURN]) {
			return 0;
		}
	}
	return 0;
}
