#include "building.h"

BuildingWindow::BuildingWindow(std::mt19937 rand, int x, int y, int w, int h) : rand(rand), x(x), y(y), w(w), h(h) {
	for (int i = 0; i < w * h; i++) {
		state.push_back(0);
	}
	nextLightTime = rand() % 2000 + 2000;
	fadeOutTime = rand() % 200 + 100;
	lightBias = rand() % 3 + 2;
}

void BuildingWindow::Update(Uint32 elapsedMs, int randomBuildingState) {
	for (auto it = state.begin(); it != state.end(); ++it) {
		if (*it <= elapsedMs) {
			*it = 0;
		}
		else {
			*it -= elapsedMs;
		}
	}

	if (randomBuildingState % lightBias == 0) {
		if (nextLightTime <= elapsedMs) {
			int enable = rand() % (w * h);
			state[enable] += rand() % 5000 + 5000;
			nextLightTime = rand() % 2000 + 2000;
		}
		else {
			nextLightTime -= elapsedMs;
		}
	}
}

void BuildingWindow::Draw(SDL_Renderer *ren) const {
	// Draw outline
	SDL_SetRenderDrawColor(ren, 0, 32, 32, 0);
	SDL_Rect rect;
	rect.h = h;
	rect.w = w + 1;
	rect.x = x;
	rect.y = y;
	SDL_RenderFillRect(ren, &rect);

	SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);
	int s = 0;
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (state[s] > 0) {
				float si = 1.0f;
				if (state[s] < fadeOutTime) {
					si = state[s] / fadeOutTime;
				}
				SDL_SetRenderDrawColor(ren, 0, 255 * si, 255 * si, 0);

				SDL_RenderDrawPoint(ren, x + i, y + j);
			}

			s++;
		}
	}
}

WarningLight::WarningLight() {

}

WarningLight::WarningLight(int maxTime, int x, int y, int w, int h) : state(false), maxTime(maxTime), currentTime(maxTime), x(x), y(y), w(w), h(h) {

}

void WarningLight::Update(Uint32 elapsedMs) {
	if (currentTime <= elapsedMs) {
		state = !state;
		currentTime = maxTime;
	}
	else {
		currentTime -= elapsedMs;
	}
}

void WarningLight::Draw(SDL_Renderer *ren) const {
	if (state) {
		//SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
		SDL_SetRenderDrawColor(ren, 255, 255, 0, 0);
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_RenderFillRect(ren, &rect);
	}
}

Building::Building(std::mt19937 rand, int ox, int oy, int w, int h, int ww, int wh, int wo, bool warningLightEnabled)
	: rand(rand), offsetX(ox), offsetY(oy), w(w), h(h), warningLightEnabled(warningLightEnabled) {
	windowCount = 0;
	int xstep = (rand() % ww) + ww + 2;
	for (int j = 0; j < h; j += wo) {
		for (int k = 0; k < w; k += xstep) {
			windowCount++;

			int x = ox + k;
			int y = oy + j;

			// Advance random state for each window
			rand();
			BuildingWindow w(rand, x, y, ww, wh);
			windows.push_back(w);
		}
	}

	int warnW = rand() % 2 + 2;
	int warnH = warnW;
	int warnX = ox + rand() % (w - warnW);
	int warnY = oy - (rand() % warnH) - (warnH / 2);
	warningLight = WarningLight(rand() % 1000 + 1000, warnX, warnY, warnW, warnH);
}

SDL_Rect Building::ToRect() const {
	SDL_Rect outline;
	outline.x = offsetX;
	outline.y = offsetY;
	outline.w = w;
	outline.h = h;
	return outline;
}

void Building::Update(Uint32 elapsedMs) {
	for (BuildingWindow& window : windows) {
		window.Update(elapsedMs, rand());
	}

	if (warningLightEnabled) {
		warningLight.Update(elapsedMs);
	}
}

void Building::Draw(SDL_Renderer *ren) const {
	SDL_Rect outline = ToRect();

	// Clear
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderFillRect(ren, &outline);

	// Draw building outlines
	//SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
	//SDL_RenderDrawRect(ren, &outline);

	// Draw windows
	for (const BuildingWindow& window : windows) {
		window.Draw(ren);
	}

	if (warningLightEnabled) {
		warningLight.Draw(ren);
	}
}
