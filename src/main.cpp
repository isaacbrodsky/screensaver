#include "SDL.h"
#include "main.h"

Scrsvr_State* Scrsvr_Init(SDL_Renderer *ren, int w, int h) {
	Scrsvr_State *state = new Scrsvr_State();
	state->w = w;
	state->h = h;
	state->i = 0;
	state->tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w, h);

	return state;
}

void Scrsvr_Update(Scrsvr_State *state) {
	state->i++;
	if (state->i >= state->w) {
		state->i = 0;
	}
}

void Scrsvr_Draw(Scrsvr_State *state, SDL_Renderer *ren) {
	if (state->i == 0) {
		SDL_SetRenderTarget(ren, state->tex);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);
	}

	SDL_SetRenderTarget(ren, state->tex);
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
	SDL_RenderDrawLine(ren, state->i, 0, state->i, state->i);

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, state->tex, NULL, NULL);
}

void Scrsvr_Destroy(Scrsvr_State *state) {
	SDL_DestroyTexture(state->tex);
	delete state;
}