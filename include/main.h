#pragma once

#include <random>
#include <vector>
#include <string>
#include "SDL.h"
#include "charrender.h"

class Scrsvr_State;

class Updater {
public:
	virtual bool Update(Uint32 elapsedMs, Scrsvr_State& state) = 0;
};

class Scrsvr_State {
private:
	SDL_Texture* tex;

	// State to draw
	Uint32 totalTime;
	Uint32 frameNumber;

	Updater *updater;

	std::string msg;
	int msgX, msgY;
	int msgDeltaX, msgDeltaY;

public:
	Scrsvr_State(SDL_Renderer* ren, int w, int h, int scaling, int wScale, int hScale);
	~Scrsvr_State();

	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer* ren, const CharRender* charRender) const;

	int w, h;
	std::mt19937 rand;
	std::vector<Uint16> chars;
};
