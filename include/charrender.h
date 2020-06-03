#pragma once

#include <random>
#include <list>
#include "SDL.h"

class CharRender {
private:
	int scaling;
    int w, h;
	SDL_Texture *tex;

public:
    CharRender(SDL_Renderer *ren, int scaling);
	~CharRender();

	void Draw(SDL_Renderer *ren, int x, int y, int ch, int col) const;
};
