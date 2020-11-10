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
 
/*grafix functions*/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "gamedefs.h"
#include "grafix.h"

SDL_Surface *penguin;
SDL_Surface *fish_surf;
SDL_Surface *seabird_surf;
SDL_Surface *title_surf;
SDL_Surface *mount_surf;
TTF_Font *font;

SDL_Surface *ocean;
SDL_Surface *sky;

int init_grafix()
{
	SDL_Surface *temp, *icon;
	Uint32 colorkey;

	/*data_dir = strdup(DATADIR);*/

	/* try to set 640x480 hicolor and use double buffering*/
	screen = SDL_SetVideoMode(640,480,16,SDL_DOUBLEBUF);
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_WM_SetCaption("Peng Swim", "Peng Swim");
	
	/*Load mountain bitmap.*/
	temp = IMG_Load(load_file("images/icon.png"));
	if (temp == NULL) {
		printf("Unable to load icon bitmap.\n");
		return 1;
	}
	
	colorkey = SDL_MapRGB(temp->format, 128, 255, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, colorkey);
	
	icon = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if (icon == NULL) {
		printf("Unable to convert icon bitmap.\n");
		return 1;
	}

	SDL_WM_SetIcon(icon,NULL);
	
	SDL_FreeSurface(icon);
	
	/* Load the background and foreground*/
	temp = IMG_Load(load_file("images/sea3.png"));
	ocean = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	if (ocean == NULL) {
		printf("Unable to load sea bitmap.\n");
		return 1;
	}
	
	temp = IMG_Load(load_file("images/sky2.png"));

	sky = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if (sky == NULL) {
		printf("Unable to load sky bitmap.\n");
		return 1;
	}

	/*Load mountain bitmap.
	temp = IMG_Load(load_file("images/mountains.png"));
	if (temp == NULL) {
		printf("Unable to load mountain bitmap.\n");
		return 1;
	}
	
	colorkey = SDL_MapRGB(temp->format, 128, 255, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);*/
	
	/*set the mountains to temp.  doing this after the colorkey can help acceleration
	mount_surf = SDL_DisplayFormat(temp);
	
	if (mount_surf ==NULL) {
		printf("Unable to convert mountain bitmap.\n");
		return 1;
	}

	*/
	
	/* Load the bitmap */
	temp = IMG_Load(load_file("images/penguin-low2.png"));
	if (temp == NULL) {
		printf("Unable to load penguin bitmap.\n");
		return 1;
	}
	
	colorkey = SDL_MapRGB(temp->format, 128, 255, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey);
	
	/*set the penguin to temp.  doing this after the colorkey can help acceleration*/
	penguin = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	if (penguin ==NULL) {
		printf("Unable to convert penguin bitmap.\n");
		return 1;
	}

	/* Load the bitmap */
	temp = IMG_Load(load_file("images/fish3.png"));
	if (temp == NULL) {
		printf("Unable to load fish bitmap.\n");
		return 1;
	}
	
	colorkey = SDL_MapRGB(temp->format, 128, 255, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey);
	
	/*set the fish to temp.  doing this after the colorkey can help acceleration*/
	fish_surf = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	if (fish_surf ==NULL) {
		printf("Unable to convert fish bitmap.\n");
		return 1;
	}
	
	/* Load the bitmap */
	temp = IMG_Load(load_file("images/seabird.png"));
	if (temp == NULL) {
		printf("Unable to load seabird bitmap.\n");
		return 1;
	}
	
	colorkey = SDL_MapRGB(temp->format, 128, 255, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	/*set the seabird to temp.  doing this after the colorkey can help acceleration*/
	seabird_surf = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	if (seabird_surf ==NULL) {
		printf("Unable to convert seabird bitmap.\n");
		return 1;
	}

	/* Load the bitmap */
	temp = IMG_Load(load_file("images/ice2.png"));
	if (temp == NULL) {
		printf("Unable to load ice bitmap.\n");
		return 1;
	}
	
	/*set the ice to temp.*/
	ice_surf = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	if (ice_surf ==NULL) {
		printf("Unable to convert ice bitmap.\n");
		return 1;
	}

	/* Load the bitmap */
	temp = IMG_Load(load_file("images/title.png"));
	if (temp == NULL) {
		printf("Unable to load title bitmap.\n");
		return 1;
	}
	
	colorkey = SDL_MapRGB(temp->format, 128, 255, 255);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	/*set the title to temp.  doing this after the colorkey can help acceleration*/
	title_surf = SDL_DisplayFormat(temp);
	
	if (title_surf ==NULL) {
		printf("Unable to convert title bitmap.\n");
		return 1;
	}

	SDL_FreeSurface(temp);
	
	font = TTF_OpenFont(FONT_FILE,FONT_SIZE);
}

void kill_grafix() 
{
	TTF_CloseFont(font);
	SDL_FreeSurface(title_surf);
	SDL_FreeSurface(sky);
	SDL_FreeSurface(ocean);
	SDL_FreeSurface(penguin);
	SDL_FreeSurface(fish_surf);
	SDL_FreeSurface(seabird_surf);
	SDL_FreeSurface(mount_surf);
	SDL_FreeSurface(ice_surf);
	SDL_FreeSurface(screen);
}

void draw_ocean()
{
	int x,y;
	SDL_Rect src,dest;
/*	,sky_rect;
	Uint32 color;
	
	sky_rect.x = 0;
	sky_rect.y = 0;
	sky_rect.w = screen->w;
	sky_rect.h = screen->h/2;

	color = SDL_MapRGB(screen->format,0,153,255);

	SDL_FillRect(screen, &sky_rect, color); */
	
/*	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h/2;
	
	dest = src;
	
	SDL_BlitSurface(sky, &src, screen, &dest);
*/

	for (y=(screen->h/2)/ocean->h; y <= ((int)screen->h/ocean->h);y++)
	{
		for (x=0; x <= ((int)screen->w/ocean->w);x++)
		{
			src.x = 0;
			src.y = 0;
			src.w = ocean->w;
			src.h = ocean->h;
	
			dest.x = x * ocean->w;
			dest.y = y * ocean->h + (screen->h/2) % ocean->h;
			dest.w = screen->w;
			dest.h = screen->h;
		
			/*SDL_SetAlpha(background, 0, 0);*/

			SDL_BlitSurface(ocean, &src, screen, &dest);
		}
	}
}


void draw_sky()
{
	int x,y;
	SDL_Rect src,dest;
	
	for (y=0; y <= floor((screen->h/sky->h)/2);y++)
	{
		for (x=0; x <= ((int)screen->w/sky->w);x++)
		{
			src.x = 0;
			src.y = 0;
			src.w = sky->w;
			if ((y * sky->h) >= floor((screen->h/sky->h)/2) * sky->h) {
				src.h = (screen->h/2) % sky->h;
			} else {
				src.h = sky->h;
			}
			
			dest.x = x * sky->w;
			dest.y = y * sky->h;
			dest.w = screen->w;
			dest.h = screen->h;
		
			/*SDL_SetAlpha(background, 0, 0);*/

			SDL_BlitSurface(sky, &src, screen, &dest);
		}
	}
	/*
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = mount_surf->h;
	
	dest.x = 0;
	dest.y = screen->h/2 - mount_surf->h;
	dest.w = screen->w;
	dest.h = screen->h;

	SDL_BlitSurface(mount_surf, &src, screen, &dest);
	*/
}

void draw_penguins(double angle)
{
	int offset;
	/*int decimal,sign;
	char* time_string;*/
	SDL_Rect src,dest;

	angle = 90 + (-1 * angle);

    if (angle < 0) angle += 360;
    if (angle >= 360) angle -= 360;

	offset = (int)angle/15 * 64;
	
	src.x = offset;
	src.y = 0;
	src.w = 64;
	src.h = 64;
	
	/*penguins position is its CENTER.  this calculates the upper left corner */
	dest.x = penguins[0].x - 32;
	dest.y = penguins[0].y - 32;
	dest.w = screen->w;
	dest.h = screen->h;
	
	SDL_BlitSurface(penguin, &src, screen, &dest);
	
	/*time_string = fcvt((double)angle, 0, &decimal, &sign);
	draw_text(time_string,5,200);*/

}

void draw_fish(int active_fish)
{
	int offset, i, angle;
	SDL_Rect src,dest;

	for (i=0;i<active_fish;i++) {
		if (fish[i].dx < 0) {
			offset = fish_surf->w/2;
		} else {
			offset = 0;
		}
		
		angle = 270 + fish[i].ang;

		if (angle < 0) angle += 360;
		if (angle >= 360) angle -= 360;	

		offset = (int)angle/15 * 32;

		src.x = offset;
		src.y = 0;
		src.w = fish_surf->w/24;
		src.h = fish_surf->h;
		
		/*fish position is its CENTER.  this calculates the upper left corner */
		dest.x = fish[i].x - fish_surf->w/48;
		dest.y = fish[i].y - fish_surf->h/2;
		dest.w = screen->w;
		dest.h = screen->h;
		
		SDL_BlitSurface(fish_surf, &src, screen, &dest);
	}
}

void draw_seabirds(int active_seabirds, double time_scale)
{
	int offset, i;
	SDL_Rect src,dest;

	for (i=0;i<active_seabirds;i++) {
		if (seabirds[i].dx < 0) {
			offset = 0;
		} else {
			offset = seabird_surf->w/2;
		}
		
		if ((SDL_GetTicks() + seabirds[i].birth)%600 >= 450) {
			offset += seabird_surf->w/4;
		}
		
		src.x = offset;
		src.y = 0;
		src.w = seabird_surf->w/4;
		src.h = seabird_surf->h;
		
		/*seabird position is its CENTER.  this calculates the upper left corner */
		dest.x = seabirds[i].x - seabird_surf->w/4;
		dest.y = seabirds[i].y - seabird_surf->h/2;
		dest.w = screen->w;
		dest.h = screen->h;
		
		SDL_BlitSurface(seabird_surf, &src, screen, &dest);
	}
}

void draw_ice() 
{
	int i;
	SDL_Rect src,dest;

	for (i=0;i<MAX_ICE;i++) {
		if (ice[i].birth == 0) continue;
		src.x = 0;
		src.y = 0;
		src.w = ice_surf->w;
		src.h = ice_surf->h;
		
		/*ice position is its CENTER.  this calculates the upper left corner */
		dest.x = ice[i].x - ice_surf->w/2;
		dest.y = ice[i].y - ice_surf->h/2;
		dest.w = screen->w;
		dest.h = screen->h;
		
		SDL_BlitSurface(ice_surf, &src, screen, &dest);
	}
}

void draw_text(char *text,int x, int y)
{
	SDL_Surface *temp, *text_box;
	SDL_Rect src,dest;
	SDL_Color text_color, shade_color;

	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
	
	shade_color.r = 0;
	shade_color.g = 128;
	shade_color.b = 128;
	
	temp = TTF_RenderText_Shaded(font, text, text_color, shade_color);
	text_box = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);
		
	src.x = 0;
	src.y = 0;
	src.w = text_box->w;
	src.h = text_box->h;
	
	dest.x = x - text_box->w/2;
	dest.y = y - text_box->h/2;
	dest.w = screen->w;
	dest.h = screen->h;
	
	SDL_BlitSurface(text_box, &src, screen, &dest);
	SDL_FreeSurface(text_box);
}


void draw_text_color(char *text,int x, int y, SDL_Color text_color, SDL_Color shade_color)
{
	SDL_Surface *temp, *text_box;
	SDL_Rect src,dest;

	temp = TTF_RenderText_Shaded(font, text, text_color, shade_color);
	text_box = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);
		
	src.x = 0;
	src.y = 0;
	src.w = text_box->w;
	src.h = text_box->h;
	
	dest.x = x - text_box->w/2;
	dest.y = y - text_box->h/2;
	dest.w = screen->w;
	dest.h = screen->h;
	
	SDL_BlitSurface(text_box, &src, screen, &dest);
	SDL_FreeSurface(text_box);
}

void draw_score(int score, int time_left)
{
	SDL_Surface *temp, *score_box, *time_box;
	char *time_text, *score_text;
	SDL_Rect src,dest;
	SDL_Color text_color, shade_color;
	int decimal,sign,fontx,fonty;
	Uint32 color;

	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
	
	shade_color.r = 0;
	shade_color.g = 128;
	shade_color.b = 128;

	TTF_SizeText(font,"0",&fontx,&fonty);

	src.x = 3 * screen->w/4;
	src.y = 0;
	src.w = screen->w/4;
	src.h = fonty;
	
	color = SDL_MapRGB(screen->format,shade_color.r,shade_color.g,shade_color.b);

	SDL_FillRect(screen, &src, color);

	score_text = fcvt((double)score, 0, &decimal, &sign);
	temp = TTF_RenderText_Shaded(font, score_text, text_color, shade_color);
	score_box = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	src.x = 0;
	src.y = 0;
	src.w = score_box->w;
	src.h = score_box->h;
	
	dest.x = 3 * screen->w/4 + 40;
	dest.y = 0;
	dest.w = screen->w;
	dest.h = screen->h;
	
	SDL_BlitSurface(score_box, &src, screen, &dest);

	time_text = fcvt((double)time_left, 0, &decimal, &sign);
	temp = TTF_RenderText_Shaded(font, time_text, text_color, shade_color);
	time_box = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	src.x = 0;
	src.y = 0;
	src.w = time_box->w;
	src.h = time_box->h;
	
	dest.x = 3 * screen->w/4 + 80;
	dest.y = 0;
	dest.w = screen->w;
	dest.h = screen->h;
	
	SDL_BlitSurface(time_box, &src, screen, &dest);

	src.x = fish_surf->w/4;
	src.y = 5;
	src.w = fish_surf->w/24;
	src.h = fish_surf->h - 5;
		
	dest.x = 3 * screen->w/4 + 4;
	dest.y = 0;
	dest.w = screen->w;
	dest.h = screen->h;
		
	SDL_BlitSurface(fish_surf, &src, screen, &dest);

	SDL_FreeSurface(score_box);
	SDL_FreeSurface(time_box);
}

void draw_title()
{
	SDL_Rect src,dest, back;
	Uint16 color;
	int color_offset;
	
	back.x = screen->w/2 - title_surf->w/2;
	back.y = screen->h/2 - title_surf->h/2;
	back.w = title_surf->w;
	back.h = title_surf->h;

	src.x = 0;
	src.y = 0;
	src.w = title_surf->w;
	src.h = title_surf->h;
		
	/*title position is its CENTER.  this calculates the upper left corner */
	dest.x = screen->w/2 - title_surf->w/2;
	dest.y = screen->h/2 - title_surf->h/2;
	dest.w = screen->w;
	dest.h = screen->h;
	
	color_offset = (int)((sin((double)((SDL_GetTicks() % 600) * PI / 300.0)) * 16) + 16.0);
	
	color = SDL_MapRGB(screen->format,0 + color_offset,128 + color_offset,128 + color_offset);

	SDL_FillRect(screen, &back, color);
	
	SDL_BlitSurface(title_surf, &src, screen, &dest);
}

void draw_scores(
	struct hiscore_s {
		char *player_name;
		int player_score;
	} hiscore[MAX_HISCORES]) 
{
	char score[1024];
	char scores[1024*MAX_HISCORES];
	int i, start;
	int fontx,fonty;
	int decimal,sign;
	Uint32 color;
	SDL_Color text_color,shade_color;
	SDL_Rect src,dest;

	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
	
	shade_color.r = 0;
	shade_color.g = 0;
	shade_color.b = 128;

	TTF_SizeText(font,"High Scores",&fontx,&fonty);
	
	dest.x = screen->w/4;
	dest.y = screen->h/2 - (fonty * (MAX_HISCORES+2))/2;
	dest.w = screen->w/2;
	dest.h = fonty * (MAX_HISCORES+2);

	start = dest.y + fonty;

	color = SDL_MapRGB(screen->format,0,128,128);

	SDL_FillRect(screen, &dest, color);
		
	draw_text("High Scores",screen->w/2,start);
	for (i=0;i<MAX_HISCORES;i++) {
		draw_text(hiscore[i].player_name,screen->w*3/8,start + (i+1)*fonty);
		draw_text(fcvt((double)hiscore[i].player_score,0,&decimal,&sign),screen->w*5/8,start + (i+1)*fonty);
	}
	
}
