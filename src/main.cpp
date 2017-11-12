#include <cmath>
#include <chrono>
#include "SDL.h"
#include "main.h"

int min(int a, int b) {
	if (a > b) {
		return b;
	}
	else {
		return a;
	}
}

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

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

void Building::Update() {

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

Scrsvr_State::Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling) : w(w / scaling), h(h / scaling), rand(std::chrono::system_clock::now().time_since_epoch().count()) {
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w / scaling, h / scaling);

	maxStars = rand() % 100 + 100;

	numBuildings = rand() % 20 + 5;
	int offsetX = 0;
	for (int i = 0; i < numBuildings; i++) {
		int bw = rand() % 70 + 60;
		int bh = rand() % (h / scaling);
		int offsetY = (h / scaling) - bh;
		Building building(rand, offsetX, offsetY, bw, bh, max(rand() % 5, 1), max(rand() % 5, 1), rand() % 10 + 1);
		buildings.push_back(building);
		offsetX += bw;
	}
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
}

void Scrsvr_State::Update() {
	bool newStarValid = true;
	SDL_Point newStar = { rand() % w, rand() % h };

	for (Building& building : buildings) {
		building.Update();

		SDL_Rect rect = building.ToRect();
		if (SDL_PointInRect(&newStar, &rect)) {
			newStarValid = false;
		}
	}

	if (newStarValid) {
		stars.push_front(newStar);

		if (stars.size() > maxStars) {
			stars.pop_back();
		}
	}
}

void Scrsvr_State::Draw(SDL_Renderer *ren) const {
	SDL_SetRenderTarget(ren, tex);

	for (const Building& building : buildings) {
		building.Draw(ren);
	}

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
	for (const SDL_Point& star : stars) {
		SDL_RenderDrawPoint(ren, star.x, star.y);
	}

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}