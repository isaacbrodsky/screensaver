#include <string>
#include <chrono>
#include "SDL.h"
#include "main.h"
#include "random.h"
#include "text.h"

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
    frameNumber(0),
    updater(nullptr) {
	tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w * wScale, h * hScale);
    chars.resize(w * h);

    msgDeltaX = rand() & 1 ? 1 : -1;
    msgDeltaY = rand() & 1 ? 1 : -1;
    msg = " " + std::string(SDL_GetPlatform()) + " ";
    msgX = rand() % (w - msg.size() - 2);
    msgY = rand() % h;
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
    if (updater) {
        delete updater;
    }
}

void Scrsvr_State::Update(Uint32 elapsedMs) {
	totalTime += elapsedMs;
    if (totalTime > FRAME_TIME) {
        totalTime -= FRAME_TIME;

        if (!updater || !updater->Update(elapsedMs, *this)) {
            if (updater) {
                delete updater;
            }
            if (rand() % 10 == 0) {
                // Weight against TextUpdater
                updater = new TextUpdater(*this);
            }
            else {
                updater = new RandomUpdater(*this);
            }
        }

        // advance message
        msgX += msgDeltaX;
        msgY += msgDeltaY;
        if (msgX + msg.size() >= w || msgX <= 0) {
            msgX = max(0, min(msgX, w - msg.size()));
            msgDeltaX = -msgDeltaX;
        }
        if (msgY + 1 >= h || msgY <= 0) {
            msgY = max(0, min(msgY, h - 1));
            msgDeltaY = -msgDeltaY;
        }
    }
}

void Scrsvr_State::Draw(SDL_Renderer *ren, const CharRender *charRender) const {
	SDL_SetRenderTarget(ren, tex);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderClear(ren);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int idx = i + j * w;
            auto toDraw = chars[idx];
            charRender->Draw(ren, i, j, toDraw & 0xFF, ((toDraw >> 8) & 0xFF));
        }
    }

    charRender->Clear(ren, msgX, msgY, msg.size(), 0x10);
    charRender->Draw(ren, msgX, msgY, msg, 0x0F);

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}