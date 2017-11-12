#pragma once

#include "SDL.h"

struct Scrsvr_State {
	int w, h;
	int i;
	SDL_Texture *tex;
};

Scrsvr_State* Scrsvr_Init(SDL_Renderer *ren, int w, int h);
void Scrsvr_Update(Scrsvr_State *state);
void Scrsvr_Draw(Scrsvr_State *state, SDL_Renderer *ren);
void Scrsvr_Destroy(Scrsvr_State *state);