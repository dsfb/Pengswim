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
 
#ifndef GAMELOOP_H
#define GAMELOOP_H
#define MAX_LINE_WIDTH 20
#define MAX_LINES 15
#define MAX_LEVELS 10




void init_ice(int active_fish);
void init_penguins();

static void move_penguins(double time_delay, int peng_control);

void init_fish(int active_fish);

static void move_fish(double time_delay);
void delete_fish(int index);
int check_ice(int x, int y, float dist);
int check_ice2(int x, int y, float dist, int * ice_x, int * ice_y);
int check_penguins(int x, int y, float dist);
int gameloop();
static int start_fish = 2;
static int active_fish;
static int active_seabirds = 0;
#endif
