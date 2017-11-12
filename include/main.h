#pragma once

#include <random>
#include "SDL.h"

class Building {
public: // TODO
	std::mt19937& rand;
	int w, h;
	int windowWidth;
	int windowHeight;
	int windowOffset;

public:
	Building(std::mt19937& rand, int w, int h, int windowWidth, int windowHeight, int windowOffset);

	void Update();
	void Draw(SDL_Renderer *ren, int offsetX, int screenHeight);
};

class Scrsvr_State {
private:
	int w, h;
	SDL_Texture *tex;

	std::mt19937 rand;

	// Current drawing state
	int i;

	// State to draw
	int numBuildings;
	std::vector<Building> buildings;

public:
	Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling);
	~Scrsvr_State();

	void Update();
	void Draw(SDL_Renderer *ren);
};
