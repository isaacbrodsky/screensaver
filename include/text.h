#pragma once

#include "main.h"

class TextUpdater : public Updater {
private:
	Uint32 frameNumber;
	Uint32 currText;
	int startX, startY;
	int xOffset;
	Uint16 color;

public:
	TextUpdater(Scrsvr_State& state);

	bool Update(Uint32 elapsedMs, Scrsvr_State& state);
};

