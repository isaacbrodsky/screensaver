#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "charrender.h"

using namespace std;

CharRender::CharRender(SDL_Renderer *ren, int scaling)
	: scaling(scaling) {
	for (int i = 0; i < 256; i++) {
        string path = "char/" + to_string(i) + ".png";
        auto loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
            throw std::exception("couldn't load surface");
        }

        auto tex = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if (tex == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            throw std::exception("couldn't load texture");
        }

        texes.push_back(tex);
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    }
}

CharRender::~CharRender() {
    for (auto tex : texes) {
        SDL_DestroyTexture(tex);
    }
}

void CharRender::Draw(SDL_Renderer *ren, int x, int y, int ch, int col) const {
    SDL_Rect rect = { x * w * scaling, y * h * scaling, w * scaling, h * scaling };
    SDL_RenderCopy(ren, texes[ch], NULL, &rect);
}

void CharRender::Draw(SDL_Renderer *ren, int x, int y, string str, int col) const {
    for (char ch : str) {
        Draw(ren, x++, y, ch, col);
    }
}