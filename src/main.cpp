#include <algorithm>
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
	: w(w / scaling), h(h / scaling), rand(std::chrono::system_clock::now().time_since_epoch().count()), totalTime(0) {
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w / scaling, h / scaling);

	starLife = rand() % 30000 + 10000;
	nextStarTime = rand() % 500;
	maxStars = rand() % 100 + 75;

	int maxHeightBuilding = -1;
	int buildingIdx = 0;
	for (int offsetX = 0; offsetX <= w / scaling; ) {
		int bw = rand() % 50 + 40;
		int bh = rand() % (h / (scaling * 2));
		int offsetY = (h / scaling) - bh;
		Building building(rand, offsetX, offsetY, bw, bh, max(rand() % 5, 1), max(rand() % 5, 1), rand() % 10 + 1, false);
		buildings.push_back(building);
		offsetX += bw - (rand() % (bw / 2));
		if (maxHeightBuilding == -1 || building.h > buildings[maxHeightBuilding].h) {
			maxHeightBuilding = buildingIdx;
		}
		buildingIdx++;
	}
	if (maxHeightBuilding != -1) {
		buildings[maxHeightBuilding].warningLightEnabled = true;
	}

	// TODO: uses unknown random state
	std::random_shuffle(buildings.begin(), buildings.end());
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
}

void Scrsvr_State::Update(Uint32 elapsedMs) {
	totalTime += elapsedMs;
	bool newStarValid = true;
	SDL_Point newStarLoc = { rand() % w, rand() % h };
	Star newStar(newStarLoc, rand() % starLife);
	if (nextStarTime >= elapsedMs) {
		// It's not time to create the next star yet.
		newStarValid = false;
		nextStarTime -= elapsedMs;
	}
	else {
		nextStarTime = rand() % 500;
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
	// TODO doesn't handle rollover
	float timeCoeff = 1.0f - (totalTime / 60000.0f);
	if (timeCoeff > 1)
		timeCoeff = 1;
	if (timeCoeff < 0)
		timeCoeff = 0;
	int r = timeCoeff * 96;
	int b = timeCoeff * 127;
	SDL_SetRenderDrawColor(ren, r, 0, b, 0);
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