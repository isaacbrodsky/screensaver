#include "building.h"

Building::Building(std::mt19937& rand, int ox, int oy, int w, int h, int ww, int wh, int wo)
	: rand(rand), offsetX(ox), offsetY(oy), w(w), h(h), windowWidth(ww), windowHeight(wh), windowOffset(wo) {

}

SDL_Rect Building::ToRect() const {
	SDL_Rect outline;
	outline.x = offsetX;
	outline.y = offsetY;
	outline.w = w;
	outline.h = h;
	return outline;
}

void Building::Update(Uint32 elapsedMs) {

}

void Building::Draw(SDL_Renderer *ren) const {
	SDL_Rect outline = ToRect();

	// Clear
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderFillRect(ren, &outline);

	// Draw building outlines
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
	SDL_RenderDrawRect(ren, &outline);

	// Draw window outlines
	SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);
	for (int j = 0; j < h; j += windowOffset) {
		for (int k = 0; k < w; k += windowWidth * 2) {
			if (rand() % 2 == 0) {
				continue;
			}

			SDL_Rect rect;
			rect.h = windowHeight;
			rect.w = windowWidth + 1;
			rect.x = offsetX + k;
			rect.y = offsetY + j;
			SDL_RenderDrawRect(ren, &rect);
		}
	}
}
