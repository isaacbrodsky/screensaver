#include "building.h"

Window::Window(std::mt19937 rand, int x, int y, int w, int h) : rand(rand), x(x), y(y), w(w), h(h) {
	for (int i = 0; i < w * h; i++) {
		state.push_back(0);
	}
}

void Window::Update(Uint32 elapsedMs) {

}

void Window::Draw(SDL_Renderer *ren) const {
	SDL_SetRenderDrawColor(ren, 0, 255, 255, 0);

	SDL_Rect rect;
	rect.h = h;
	rect.w = w + 1;
	rect.x = x;
	rect.y = y;
	SDL_RenderDrawRect(ren, &rect);
}

Building::Building(const std::mt19937& rand, int ox, int oy, int w, int h, int ww, int wh, int wo)
	: rand(rand), offsetX(ox), offsetY(oy), w(w), h(h) {
	windowCount = 0;
	for (int j = 0; j < h; j += wo) {
		for (int k = 0; k < w; k += ww * 2) {
			windowCount++;

			int x = ox + k;
			int y = oy + j;

			Window w(rand, x, y, ww, wh);
			windows.push_back(w);
		}
	}
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
	for (Window& window : windows) {
		window.Update(elapsedMs);
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
	for (const Window& window : windows) {
		window.Draw(ren);
	}
}
