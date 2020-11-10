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
 
#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL/SDL.h>

/* Particle data type. */
typedef struct particle_s {
    double x,y;		        /* coordinates of the particle */
    double dx,dy; 		/* velocity of the particle */
    int birth;		/* birth tick */
    int dur;		/* ticks to live */
	Uint8 type, r, g, b;		/* particle type*/
} particle_t, *particle_p;

void DrawParticles(SDL_Surface *dest, int camera_x, int camera_y);
/* Draws all active and visible particles to the given surface. */

void UpdateBubbles(double time_scale);
/* Updates the position of each bubble particle in the system, and removes bubbles
   that have lost their life or have crossed into the air. 
	This should typically be called once per frame. */

void CreateBubble(int x, int y);
/* Creates a bubble at the given (x,y) position in the world.
   Assigns each a random duration. */


void UpdateSplashes(double time_scale);
/* Updates the position of each splash in the system, and removes particles
   that have hit the water. This should typically be called once per frame. */

void CreateSplashes(int x, int y);
/* Creates a splash at the given (x,y) position in the world.
  */

#endif

