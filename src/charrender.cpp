#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "charrender.h"

using namespace std;

SDL_Color fgcols[] = { {255, 255, 255, 255}, {0, 0, 0, 0} };
SDL_Color bgcols[] = { {0, 0, 0, 0}, {255, 255, 255, 255} };

SDL_Color cols[] = {
    {0,0,0,255},
    {0,0,168,255},
    {0,168,0,255},
    {0,168,168,255},
    {168,0,0,255},
    {168,0,168,255},
    {168,87,0,255},
    {168,168,168,255},
    {87,87,87,255},
    {87,87,255,255},
    {87,255,87,255},
    {87,255,255,255},
    {255,87,87,255},
    {255,87,255,255},
    {255,255,87,255},
    {255,255,255,255}
};

CharRender::CharRender(SDL_Renderer *ren, int scaling)
	: scaling(scaling) {
    auto fgpal = SDL_AllocPalette(2);
    SDL_SetPaletteColors(fgpal, fgcols, 0, 2);
    auto bgpal = SDL_AllocPalette(2);
    SDL_SetPaletteColors(bgpal, bgcols, 0, 2);

	for (int i = 0; i < 256; i++) {
        string path = "char/" + to_string(i) + ".png";
        auto loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL) {
            std::cout << "Unable to load: " << path << " Error: " << IMG_GetError() << std::endl;
            throw std::exception("couldn't load surface");
        }

        SDL_SetSurfacePalette(loadedSurface, fgpal);

        auto tex = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if (tex == NULL) {
            std::cout << "Unable to create fg texture: " << path << " Error: " << SDL_GetError() << std::endl;
            throw std::exception("couldn't load texture");
        }

        texes.push_back(tex);

        SDL_SetSurfacePalette(loadedSurface, bgpal);

        tex = SDL_CreateTextureFromSurface(ren, loadedSurface);
        if (tex == NULL) {
            std::cout << "Unable to create bg texture: " << path << " Error: " << SDL_GetError() << std::endl;
            throw std::exception("couldn't load bg texture");
        }

        bgtexes.push_back(tex);

        SDL_QueryTexture(tex, NULL, NULL, &w, &h);

        SDL_FreeSurface(loadedSurface);
    }

    SDL_FreePalette(fgpal);
    SDL_FreePalette(bgpal);
}

CharRender::~CharRender() {
    for (auto tex : texes) {
        SDL_DestroyTexture(tex);
    }
    for (auto tex : bgtexes) {
        SDL_DestroyTexture(tex);
    }
}

void CharRender::Clear(SDL_Renderer *ren, int x, int y, int count, int col) const {
    SDL_Rect rect = { x * w * scaling, y * h * scaling, w * scaling * count, h * scaling };

    auto bgcol = cols[(col & 0xF0) >> 4];

    SDL_SetRenderDrawColor(ren, bgcol.r, bgcol.g, bgcol.b, 0);
    SDL_RenderFillRect(ren, &rect);
}

void CharRender::Draw(SDL_Renderer *ren, int x, int y, int ch, int col) const {
    SDL_Rect rect = { x * w * scaling, y * h * scaling, w * scaling, h * scaling };

    auto tex = texes[ch];
    auto bgtex = bgtexes[ch];
    
    auto fgcol = cols[col & 0x0F];
    SDL_SetTextureBlendMode(tex, SDL_BlendMode::SDL_BLENDMODE_ADD);
    SDL_SetTextureColorMod(tex, fgcol.r, fgcol.g, fgcol.b);
    SDL_RenderCopy(ren, tex, NULL, &rect);

    auto bgcol = cols[(col & 0xF0) >> 4];
    SDL_SetTextureBlendMode(bgtex, SDL_BlendMode::SDL_BLENDMODE_ADD);
    SDL_SetTextureColorMod(bgtex, bgcol.r, bgcol.g, bgcol.b);
    SDL_RenderCopy(ren, bgtex, NULL, &rect);
}

void CharRender::Draw(SDL_Renderer *ren, int x, int y, string str, int col) const {
    for (char ch : str) {
        Draw(ren, x++, y, ch, col);
    }
}

int CharRender::GetWidth() const {
    return w;
}

int CharRender::GetHeight() const {
    return h;
}
