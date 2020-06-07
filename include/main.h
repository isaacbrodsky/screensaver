#pragma once

#include <random>
#include <list>
#include "SDL.h"
#include "charrender.h"

class Scrsvr_State {
private:
	int w, h;
	SDL_Texture *tex;

	std::mt19937 rand;

	// State to draw
	Uint32 totalTime;
    Uint32 frameNumber;

public:
	Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling, int wScale, int hScale);
	~Scrsvr_State();

	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer *ren, const CharRender *charRender) const;
};
