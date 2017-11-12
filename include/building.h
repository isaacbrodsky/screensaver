#pragma once

#include <random>
#include "SDL.h"

class Window {
public:
	std::mt19937 rand;

	int x;
	int y;
	int w;
	int h;
	std::vector<int> state;
	Uint32 nextLightTime;

public:
	Window(std::mt19937 rand, int x, int y, int w, int h);

	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer *ren) const;
};

class Building {
public: // TODO
	int offsetX;
	int offsetY;

	std::mt19937 rand;
	int w, h;

	int windowCount;
	std::vector<Window> windows;

public:
	Building(std::mt19937 rand, int offsetX, int offsetY, int w, int h, int windowWidth, int windowHeight, int windowOffset);
	
	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer *ren) const;

	SDL_Rect ToRect() const;
};
