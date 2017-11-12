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

Scrsvr_State::Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling) 
	: w(w / scaling), h(h / scaling), rand(std::chrono::system_clock::now().time_since_epoch().count()) {
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w / scaling, h / scaling);

	Uint32 starLife = rand() % 15000 + 2000;
	nextStarTime = rand() % starLife;
	maxStars = rand() % 100 + 75;

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

void Scrsvr_State::Update(Uint32 elapsedMs) {
	bool newStarValid = true;
	SDL_Point newStarLoc = { rand() % w, rand() % h };
	Star newStar(newStarLoc, rand() % starLife);
	if (nextStarTime >= elapsedMs) {
		// It's not time to create the next star yet.
		newStarValid = false;
		nextStarTime -= elapsedMs;
	}
	else {
		nextStarTime = rand() % starLife + 2000;
	}

	for (Building& building : buildings) {
		building.Update(elapsedMs);

		SDL_Rect rect = building.ToRect();
		if (newStarValid && SDL_PointInRect(&newStar.loc, &rect)) {
			newStarValid = false;
		}
	}

	for (auto it = stars.begin(); it != stars.end(); ) {
		Star& star = *it;

		star.Update(elapsedMs);

		if (!star.IsAlive()) {
			it = stars.erase(it);
		}
		else {
			++it;
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
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);

	for (const Building& building : buildings) {
		building.Draw(ren);
	}

	for (const Star& star : stars) {
		star.Draw(ren);
	}

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}