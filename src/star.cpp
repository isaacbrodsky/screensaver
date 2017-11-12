#include "star.h"

Star::Star(const SDL_Point& point, Uint32 durationMs) : loc(point), durationMs(durationMs) {

}

void Star::Update(Uint32 elapsedMs) {
	if (durationMs <= elapsedMs) {
		durationMs = 0;
	}
	else {
		durationMs -= elapsedMs;
	}
}

void Star::Draw(SDL_Renderer* ren) const {
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
	SDL_RenderDrawPoint(ren, loc.x, loc.y);
}

bool Star::IsAlive() const {
	return durationMs != 0;
}