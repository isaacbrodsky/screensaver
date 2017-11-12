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

Building::Building(std::mt19937& rand, int w, int h, int ww, int wh, int wo)
	: rand(rand), w(w), h(h), windowWidth(ww), windowHeight(wh), windowOffset(wo) {

}

void Building::Update() {

}

void Building::Draw(SDL_Renderer *ren, int offsetX, int screenHeight) {
	// Draw building outlines
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
	SDL_RenderDrawLine(ren, offsetX, screenHeight, offsetX, screenHeight - h);
	SDL_RenderDrawLine(ren, offsetX, screenHeight - h, offsetX + w, screenHeight - h);
	SDL_RenderDrawLine(ren, offsetX + w, screenHeight, offsetX + w, screenHeight - h);

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
			rect.y = screenHeight - j;
			SDL_RenderDrawRect(ren, &rect);
		}
	}
}

Scrsvr_State::Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling) : w(w / scaling), h(h / scaling), rand(std::chrono::system_clock::now().time_since_epoch().count()) {
	i = 0;
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w / scaling, h / scaling);

	numBuildings = rand() % 20 + 5;
	for (int i = 0; i < numBuildings; i++) {
		Building building(rand, rand() % 70 + 60, rand() % h, max(rand() % 5, 1), max(rand() % 5, 1), rand() % 10 + 1);
		buildings.push_back(building);
	}
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
}

void Scrsvr_State::Update() {
	i++;
	if (i >= numBuildings) {
		i = 0;
	}

	buildings[i].Update();
}

void Scrsvr_State::Draw(SDL_Renderer *ren) {
	SDL_SetRenderTarget(ren, tex);

	if (i == 0) {
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);
	}

	int offset = 0;
	for (int j = 0; j < i; j++) {
		offset += buildings[j].w;
	}

	buildings[i].Draw(ren, offset, h);

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}