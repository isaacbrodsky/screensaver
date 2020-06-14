#include <string>
#include <chrono>
#include "SDL.h"
#include "main.h"

const Uint32 FRAME_TIME = 160;

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

int dist(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

Scrsvr_State::Scrsvr_State(SDL_Renderer *ren, int w, int h, int scaling, int wScale, int hScale) 
	: w(w / scaling),
    h(h / scaling),
    rand(std::chrono::system_clock::now().time_since_epoch().count()),
    totalTime(0),
    frameNumber(0) {
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w * wScale, h * hScale);
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
}

void Scrsvr_State::Update(Uint32 elapsedMs) {
	totalTime += elapsedMs;
    if (totalTime > FRAME_TIME) {
        totalTime -= FRAME_TIME;
        frameNumber++;
    }
}

void Scrsvr_State::Draw(SDL_Renderer *ren, const CharRender *charRender) const {
	SDL_SetRenderTarget(ren, tex);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);

    // Only red/gray backgrounds, only low intensity foreground
    /*
    int bc = (dist(0, 0, w, h) + (frameNumber / 2)) & (0xE0);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int n = ((i + (j * w)) + frameNumber + (j * frameNumber)) % 5 + 176;
            if (n == 176 + 3)
                n = 219;
            if (n == 176 + 4)
                n = 0;

            int bias = (bc >> 4) + 1;
            int c = bc | 0x07;//(n & 0x07);//((i + (j * w) + ((j * frameNumber) / bias)) & 0x07);
            charRender->Draw(ren, i, j, n, c);
        }
    }
    */

    for (int i = 0; i < frameNumber % 256; i++) {
        charRender->Draw(ren, 5 + (i % 16), 2 + (i / 16), i, ((frameNumber >> 4) & 0x0F));
    }

    std::string msg = std::string(SDL_GetPlatform());
    charRender->Clear(ren, (frameNumber % w) - 1, frameNumber % h, msg.size() + 2, 0x10);
    charRender->Draw(ren, frameNumber % w, frameNumber % h, msg, 0x0F);

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}