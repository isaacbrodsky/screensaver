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
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
}

void Scrsvr_State::Update(Uint32 elapsedMs) {
	totalTime += elapsedMs;
}

void Scrsvr_State::Draw(SDL_Renderer *ren, const CharRender *charRender) const {
	SDL_SetRenderTarget(ren, tex);
	// TODO doesn't handle rollover
	float timeCoeff = 1.0f - (totalTime / 60000.0f);
	if (timeCoeff > 1)
		timeCoeff = 1;
	if (timeCoeff < 0)
		timeCoeff = 0;

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);

    charRender->Draw(ren, 0, 0, 'A', 0x0F);
    charRender->Draw(ren, 1, 0, 'B', 0x0E);
    charRender->Draw(ren, 2, 0, 'C', 0x0D);
    charRender->Draw(ren, 0, 1, "Hello", 0x40);

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}