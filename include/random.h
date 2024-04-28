#pragma once

#include <vector>
#include "SDL.h"
#include "main.h"

class RandomUpdater : public Updater {
private:
	Uint32 frameNumber;

	Uint32 currBlockMode;
	Uint32 currBlockOffset;
	int startX, startY;

public:
	RandomUpdater(Scrsvr_State& state);

	bool Update(Uint32 elapsedMs, Scrsvr_State& state);

	std::vector<Uint16> chars;
};
