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
 
#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#define NUM_PENGUINS 1
#define MAX_SPEED 7
#define MAX_ACCEL .5
#define CHARGE_ACCEL 1
#define CHARGE_TIME 100
#define ANGLE_RATE 15
#define DRAG .05
#define PENG_UP 1
#define PENG_DOWN 2
#define PENG_LEFT 4
#define PENG_RIGHT 8
#define PENG_CHARGE 16
#define GRAVITY 4
#define BOUYANCY .5
#define TICK_DELAY 40
#ifndef FONT_FILE
#define FONT_FILE "/usr/share/fonts/default/TrueType/helb____.ttf"
#endif
#define FONT_SIZE 22
#define PI 3.141592
#define MAX_PARTICLES 5000
#define MAX_FISH 20
#define MAX_SEABIRDS 5
#define MAX_ICE 300
#define MAX_HISCORES 10
#define MAX_TIME 30
#define PLAYER_LOSE 0
#define PLAYER_WIN 1
#define PLAYER_QUIT 2
#define PLAYER_CONTINUE 3
#define MAX_SOUNDS 3
#define MAX_TRACKS 1
#define SOUND_GULP 1
#define SOUND_BURP 0
#define SOUND_SPLASH 2
#define TRACK_GAME 0
#include <sys/stat.h>

static char * data_dir = DATADIR;

static char * load_file(char *s) {
    static char retval[1024];
	struct stat statbuf;
    snprintf(retval, 1024, "%s/%s", data_dir, s);
	if (stat(retval,&statbuf) != 0) {
		snprintf(retval, 1024, "../data/%s", s);
	}
    return retval;
}

SDL_Surface *screen;
SDL_Surface *ice_surf;

/* penguin structure */
typedef struct penguin_s {
	int x,y;
	int worldx,worldy;
	int charging;
	double v;
	double dx,dy;
	double a;
	double ax,ay;
	double ang;
	int collision;
	int frame;
} penguin_t, *penguin_p;

/*Array of penguins*/
penguin_t penguins[1];

/*fish structure*/
typedef struct fish_s {
	int x,y;
	int worldx,worldy;
	double v;
	double dx,dy;
	double a;
	double ax,ay;
	double ang;
	int active;
	int collision;
	int birth;
} fish_t, *fish_p;

fish_t fish[MAX_FISH];

/*seabird structure*/
typedef struct seabird_s {
	int x,y;
	int worldx,worldy;
	double v;
	double dx,dy;
	double a;
	double ax,ay;
	double ang;
	int birth;
} seabird_t, *seabird_p;

seabird_t seabirds[MAX_FISH];

/*ice structure*/
typedef struct ice_s {
	int x,y;
	int worldx,worldy;
	int birth;
} ice_t, *ice_p;

ice_t ice[MAX_ICE];

/*hiscore structure*/


int player_state, start_time;
int score;

#endif
