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
#include <math.h>
#include <stdlib.h> 
#include "particle.h"
#include "gamedefs.h"

int active_particles = 0;
particle_t particles[MAX_PARTICLES];

static void AddParticle(particle_p particle);
static void DeleteParticle(int index);
static Uint16 CreateHicolorPixel(SDL_PixelFormat *fmt, Uint8 red, Uint8 green, Uint8 blue);

static void AddParticle(particle_p particle)
{
    /* If there are already too many particles, forget it. */
    if (active_particles >= MAX_PARTICLES) return;
	
    particles[active_particles] = *particle;
    active_particles++;
}

/* Removes a particle from the system (by index). */
static void DeleteParticle(int index)
{
    /* Replace the particle with the one at the end
       of the list, and shorten the list. */
    particles[index] = particles[active_particles-1];
    active_particles--;
}

/* Draws all active particles on the screen. */
void DrawParticles(SDL_Surface *dest, int camera_x, int camera_y)
{
    int i;
    /*,decimal,sign;
    char* time_string;*/
    Uint16 *pixels;
	
    /* Lock the target surface for direct access. */
    if (SDL_LockSurface(dest) != 0) return;
    pixels = (Uint16 *)dest->pixels;
	
    for (i = 0; i < active_particles; i++) {
	int x,y;
	Uint16 color;
		
	/* Convert world coords to screen coords. */
	x = particles[i].x - camera_x;
	y = particles[i].y - camera_y;
	if ((x < 0) || (x >= screen->w)) continue;
	if ((y < 0) || (y >= screen->h)) continue;

	/* Find the color of this particle. */
	color = CreateHicolorPixel(dest->format,
				   particles[i].r,
				   particles[i].g,
				   particles[i].b);

	/* Draw the particle. */		
	pixels[(dest->pitch/2)*y+x] = color;
    }
	
    /* Release the screen. */
    SDL_UnlockSurface(dest);

/*	time_string = fcvt((double)active_particles, 0, &decimal, &sign);
	draw_text(time_string,5,130);*/
}

/* Updates the position of each bubble particle. Kills bubbles with
   no duration left. */
void UpdateBubbles(double time_scale)
{
    int i;
	
    for (i = 0; i < active_particles; i++) {
		if (particles[i].type == 1) {
			particles[i].y += (int)particles[i].dy * time_scale;
			particles[i].dy -= BOUYANCY/2 * time_scale;			 		

			/* If the particle has faded to black, delete it.  SDL_GetTicks() - particles[i].birth >= particles[i].dur |  */
			if (particles[i].y < screen->h/2 | SDL_GetTicks() - particles[i].birth > particles[i].dur) {
				DeleteParticle(i);
			
				/* DeleteParticle replaces the current particle with the one
				 at the end of the list, so we'll need to take a step back. */
				 i--;
			}
		}
    }
}

/* Creates a bubble at a given position. */
void CreateBubble(int x, int y)
{
    particle_t particle;
	int i, birth, dur;
	
	birth = SDL_GetTicks();
	dur = rand() % 1000;

    if (rand()%2 == 0) {
 	   for (i=0; i<9; i++) {
			if ((i%3) != 1 & floor(i/3) != 1 & y >= screen->h/2) {
		 	   /* Create a big bubble at the specified point with a random duration. */
			
				particle.x = x + (i%3);
				particle.y = y + floor(i/3);
				particle.dy = (rand() % 5) / -10;
				particle.birth = birth;
				particle.dur = dur;
				particle.type = 1;
				particle.r = 255;
				particle.g = 255;
				particle.b = 255;
			
				/* Add the particle to the particle system. */
				AddParticle(&particle);
			}
		}
	} else {
		/* Create a small bubble at the specified point with a random duration. */
			
		particle.x = x;
		particle.y = y;
		particle.dy = (rand() % 5) / -10;
		particle.birth = birth;
		particle.dur = dur;
		particle.type = 1;
		particle.r = 255;
		particle.g = 255;
		particle.b = 255;
		/* Add the particle to the particle system. */
		AddParticle(&particle);
	}
}


/* Creates a bubble at a given position. */
void CreateSplash(int x, int y)
{
    particle_t particle;
	int i, j, birth, dur, px, py, pdx, pdy, color_offset, red_offset;
	
	birth = SDL_GetTicks();
	dur = rand() % 1000;

    for (i=0; i<40; i++) {
		px = x + ((rand() % 8) - 4);
		py = screen->h/2 - (rand() % 8) -1;
		pdx = (rand() % 10) - 5;
		pdy = -(rand() % 15);
		color_offset = rand() % 64;
		red_offset = 0;
		/*makes a few white splash particles*/
		if (rand() % 25 == 0) {
			color_offset = 95;
			red_offset = 128;
		}
		
	   /* Create a splash at the specified point with a random duration. */
		for (j=0;j<4;j++) {
			particle.x = px + (j % 2);
			particle.y = py - floor(j/2);
			particle.dx = pdx;
			particle.dy = pdy;
			particle.birth = birth;
			particle.dur = dur;
			particle.type = 2;
			particle.r = 32 + color_offset + red_offset;
			particle.g = 160 + color_offset;
			particle.b = 160 + color_offset;
			AddParticle(&particle);
		}
		/* Add the particle to the particle system. */
	}
}

/* Updates the position of each splash particle. Kills splashes with
   no duration left. */
void UpdateSplashes(double time_scale)
{
    int i;
	
    for (i = 0; i < active_particles; i++) {
		if (particles[i].type == 2) {
			particles[i].y += (int)particles[i].dy * time_scale;
			particles[i].x += (int)particles[i].dx * time_scale;
			particles[i].dy += GRAVITY * time_scale;			 		
	
			/* If the particle has faded to black, delete it.  SDL_GetTicks() - particles[i].birth >= particles[i].dur |  */
			if (particles[i].y > screen->h/2 | SDL_GetTicks() - particles[i].birth > particles[i].dur) {
				DeleteParticle(i);
				
			   /* DeleteParticle replaces the current particle with the one
				 at the end of the list, so we'll need to take a step back. */
			 i--;
			}
		}
    }
}


/* This is directly from another code listing. It creates a 16-bit pixel. */
static Uint16 CreateHicolorPixel(SDL_PixelFormat *fmt, Uint8 red, Uint8 green, Uint8 blue)
{
    Uint16 value;
	
    /* This series of bit shifts uses the information from the SDL_Format
       structure to correctly compose a 16-bit pixel value from 8-bit red,
       green, and blue data. */
    value = (((red >> fmt->Rloss) << fmt->Rshift) +
	     ((green >> fmt->Gloss) << fmt->Gshift) +
	     ((blue >> fmt->Bloss) << fmt->Bshift));

    return value;
}
