#pragma once

#include "SDL.h"

class Star {
public: // TODO
	SDL_Point loc;
	Uint32 durationMs;

public:
	Star(const SDL_Point& point, Uint32 durationMs);
	
	void Update(Uint32 elapsedMs);
	void Draw(SDL_Renderer* ren) const;

	bool IsAlive() const;
};
