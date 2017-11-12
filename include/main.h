#pragma once

#include <random>
#include <list>
#include "SDL.h"

class Building {
public: // TODO
	int offsetX;
	int offsetY;

	std::mt19937& rand;
	int w, h;
	int windowWidth;
	int windowHeight;
	int windowOffset;

public:
	Building(std::mt19937& rand, int offsetX, int offsetY, int w, int h, int windowWidth, int windowHeight, int windowOffset);

	void Update();
	void Draw(SDL_Renderer *ren) const;

	SDL_Rect ToRect() const;
};

class Scrsvr_State {
private:
	int w, h;
	SDL_Texture *tex;

	std::mt19937 rand;

	// State to draw
	int numBuildings;
	std::vector<Building> buildings;
	int maxStars;
	std::list<SDL_Point> stars;

public:
	Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling);
	~Scrsvr_State();

	void Update();
	void Draw(SDL_Renderer *ren) const;
};
