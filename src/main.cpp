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
    RandomizeStart();
    chars.resize(w * h);
}

Scrsvr_State::~Scrsvr_State() {
	SDL_DestroyTexture(tex);
}

void Scrsvr_State::RandomizeStart() {
    startX = rand() % w;
    startY = rand() % h;
    currBlockMode = rand();
    currBlockOffset = rand();
    msg = " " + std::string(SDL_GetPlatform()) + " ";
    msgX = rand() % (w - msg.size() - 2);
    msgY = rand() % h;
    msgDeltaX = rand() & 1 ? 1 : -1;
    msgDeltaY = rand() & 1 ? 1 : -1;
}

void Scrsvr_State::Update(Uint32 elapsedMs) {
	totalTime += elapsedMs;
    if (totalTime > FRAME_TIME) {
        totalTime -= FRAME_TIME;
        frameNumber++;
        if (frameNumber >= 256) {
            frameNumber = 0;
            RandomizeStart();
        }

        int ax = startX;
        int ay = startY;
        if (currBlockMode & 1) {
            ax += frameNumber % 16;
        } else {
            ay += frameNumber * 16;
        }
        if (currBlockMode & 2) {
            ay += frameNumber / 16;
        } else {
            ax += frameNumber * 16;
        }
        while (ax >= w) {
            ax -= w;
        }
        while (ay >= h) {
            ay -= h;
        }
        int nextChar = rand() & 0xFFFF;
        int nextOffset = frameNumber;
        if (currBlockMode & 32) {
            // Offset using a random seed too
            nextOffset += currBlockOffset;
        }
        nextOffset &= 0xFF;
        if (currBlockMode & 4) {
            nextChar = (nextChar & 0xF00F) | (nextOffset << 4);
        }
        if (currBlockMode & 8) {
            nextChar = (nextChar & 0xFF) | (nextOffset << 8);
        }
        if (currBlockMode & 16) {
            nextChar = (nextChar & 0xFF00) | nextOffset;
        }
        if (currBlockMode & 64) {
            // Only low intensity colors
            nextChar &= 0x7FFF;
        }
        chars[ax + ay * w] = nextChar;

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

char toHexChar(Uint32 i) {
    if (i < 10) {
        return '0' + i;
    } else {
        return 'A' + (i - 10);
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

    /*int z = currBlockMode;
    for (int i = 0; i < 16; i++) {
        charRender->Draw(ren, 15 - i, 0, (z & 1) ? '1' : '0', i % 4 == 0 ? 0x0E : 0x0F);
        z = z >> 1;
    }*/

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, tex, NULL, NULL);
}