#include "random.h"

RandomUpdater::RandomUpdater(Scrsvr_State& state) {
    frameNumber = 0;
    startX = state.rand() % state.w;
    startY = state.rand() % state.h;
    currBlockMode = state.rand();
    currBlockOffset = state.rand();
}

bool RandomUpdater::Update(Uint32 elapsedMs, Scrsvr_State& state) {
    frameNumber++;
    if (frameNumber >= 256) {
        frameNumber = 0;
        return false;
    }

    int ax = startX;
    int ay = startY;
    if (currBlockMode & 1) {
        ax += frameNumber % 16;
    }
    else {
        ay += frameNumber * 16;
    }
    if (currBlockMode & 2) {
        ay += frameNumber / 16;
    }
    else {
        ax += frameNumber * 16;
    }
    while (ax >= state.w) {
        ax -= state.w;
    }
    while (ay >= state.h) {
        ay -= state.h;
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
        // TODO: Does this work?
        nextChar &= 0x7FFF;
    }
    state.chars[ax + ay * state.w] = nextChar;
    return true;
}