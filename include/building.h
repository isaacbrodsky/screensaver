#pragma once

#include <random>
#include "SDL.h"

class Building {
public: // TODO
	int offsetX;
	int offsetY;

	std::mt19937 rand;
	int w, h;

	int windowCount;
	int windowWidth;
	int windowHeight;
	int windowOffset;
	std::vector<bool> windowState;

public:
	Building(const std::mt19937& rand, int offsetX, int offsetY, int w, int h, int windowWidth, int windowHeight, int windowOffset);
	
	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer *ren) const;

	SDL_Rect ToRect() const;
};
