#pragma once

#include <random>
#include <vector>
#include "SDL.h"

class CharRender {
private:
	int scaling;
    int w, h;
    std::vector<SDL_Texture*> texes;
    std::vector<SDL_Texture*> bgtexes;

public:
    CharRender(SDL_Renderer *ren, int scaling);
	~CharRender();

    void Draw(SDL_Renderer *ren, int x, int y, int ch, int col) const;
    void Draw(SDL_Renderer *ren, int x, int y, std::string str, int col) const;
};
