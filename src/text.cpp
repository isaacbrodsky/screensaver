#include "SDL.h"
#include "text.h"

const std::vector<std::string> texts = {
// William Blake, The Tyger
"Tyger Tyger, burning bright, \nIn the forests of the night;\nWhat immortal hand or eye,\nCould frame thy fearful symmetry ?\n\nIn what distant deeps or skies.\nBurnt the fire of thine eyes ?\nOn what wings dare he aspire ?\nWhat the hand, dare seize the fire ?\n\nAnd what shoulder,& what art,\nCould twist the sinews of thy heart ?\nAnd when thy heart began to beat.\nWhat dread hand ? &what dread feet ?\n\nWhat the hammer ? what the chain,\nIn what furnace was thy brain ?\nWhat the anvil ? what dread grasp.\nDare its deadly terrors clasp ?\n\nWhen the stars threw down their spears\nAnd water'd heaven with their tears:\nDid he smile his work to see ?\nDid he who made the Lamb make thee ?\n\nTyger Tyger burning bright,\nIn the forests of the night :\nWhat immortal hand or eye,\nDare frame thy fearful symmetry ? "
};

TextUpdater::TextUpdater(Scrsvr_State& state) {
    frameNumber = 0;
    currText = state.rand() % texts.size();
    startX = state.rand() % state.w;
    startY = state.rand() % state.h;
    xOffset = 0;
    color = (state.rand() % 16) << 8;
}

bool TextUpdater::Update(Uint32 elapsedMs, Scrsvr_State& state) {
    auto text = texts[currText];
    auto nextChar = text[frameNumber];
    frameNumber++;
    if (frameNumber > text.length()) {
        return false;
    }

    if (nextChar == '\n') {
        xOffset = 0;
        startY++;
        if (frameNumber >= text.length()) {
            return false;
        }
        nextChar = text[frameNumber];
        frameNumber++;
    }
    else {
        xOffset++;
    }

    int ax = startX + xOffset;
    int ay = startY;
    while (ax >= state.w) {
        ax -= state.w;
    }
    while (ay >= state.h) {
        ay -= state.h;
    }
    state.chars[ax + ay * state.w] = nextChar | color;
    return true;
}