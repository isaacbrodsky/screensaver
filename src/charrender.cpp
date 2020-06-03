#include "SDL.h"
#include "charrender.h"

CharRender::CharRender(SDL_Renderer *ren, int scaling)
	: scaling(scaling) {
	// TODO w,h,chars
}

CharRender::~CharRender() {
	// TODO SDL_DestroyTexture(tex);
}

void CharRender::Draw(SDL_Renderer *ren, int x, int y, int ch, int col) const {
	
}