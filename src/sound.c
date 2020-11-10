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
 
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "gamedefs.h"

Mix_Chunk *sounds[MAX_SOUNDS];
Mix_Music *tracks[MAX_TRACKS];
int sound_channels[MAX_SOUNDS] = {
	-1
};

char *wavfiles[MAX_SOUNDS] = {
	"sounds/burp.wav",
	"sounds/gulp.wav",
	"sounds/splash.wav"
};

char *modfiles[MAX_TRACKS] = {
	"sounds/music.it"
};

int current_track = -1;

int init_sounds() {
	int audio_rate = 22050;
	int audio_channels = 2;
	int audio_buffers = 4096;
	Uint16 audio_format = AUDIO_S16;
	int i;
	
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Can't open audio.\n");
		return 1;
	}
	
	for (i=0;i<MAX_TRACKS;i++) {
		tracks[i] = Mix_LoadMUS(load_file(modfiles[i]));
		if (tracks[i] == NULL) {
			printf("Unable to load track.\n");
			return 1;
		}
	}

	for (i=0;i<MAX_SOUNDS;i++) {
		sounds[i] = Mix_LoadWAV(load_file(wavfiles[i]));
		if (sounds[i] == NULL) {
			printf("Unable to load sound.\n");
			return 1;
		}
		Mix_VolumeChunk(sounds[i], 64);
	}
	return 0;
}

void play_sound(int index) {
	sound_channels[index] = Mix_PlayChannel(index,sounds[index],0);
}

void play_track(int index) {
	if (current_track == index) return;
	if (current_track) {
		Mix_FadeOutMusic(1000);
	}
	Mix_FadeInMusic(tracks[index],-1,1000);
	Mix_VolumeMusic(32);
	current_track = index;
}

void stop_sound(int index) {
	if (sound_channels[index] != -1) {
		Mix_HaltChannel(sound_channels[index]);
		sound_channels[index] = -1;
	}
}

void stop_track() {
	if (current_track) {
		Mix_FadeOutMusic(1000);
	}
	Mix_HaltMusic();
	current_track = -1;
}

void kill_sounds() {
	int i;
	
	stop_track();
	
	for (i=0;i<MAX_SOUNDS;i++) {
		Mix_HaltChannel(sound_channels[i]);
		Mix_FreeChunk(sounds[i]);
	}
	for (i=0;i<MAX_TRACKS;i++) {
		Mix_FreeMusic(tracks[i]);
	}
	
	Mix_CloseAudio();
}
