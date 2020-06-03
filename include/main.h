#pragma once

#include <random>
#include <list>
#include "SDL.h"

class Scrsvr_State {
private:
	int w, h;
	SDL_Texture *tex;

	std::mt19937 rand;

	// State to draw
	int maxStars;
	Uint32 nextStarTime;
	Uint32 starLife;
	Uint32 totalTime;

public:
	Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling);
	~Scrsvr_State();

	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer *ren) const;
};
