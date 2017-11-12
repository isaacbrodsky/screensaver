#include <iostream>
#include "SDL.h"
#include "winscreensaver.h"

int RunScreensaver(SDL_Window* win, SDL_Renderer* ren) {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	int i = 0;

	SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB888, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, w, h);

	bool keepRunning = true;
	while (keepRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				keepRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				keepRunning = false;
				break;
			}
		}

		SDL_SetRenderTarget(ren, tex);
		SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
		SDL_RenderDrawLine(ren, i, 0, i, i);

		SDL_SetRenderTarget(ren, NULL);
		//SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		//SDL_RenderClear(ren);

		SDL_RenderCopy(ren, tex, NULL, NULL);
		
		SDL_RenderPresent(ren);
		SDL_Delay(1);
		i++;
	}

	return 0;
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

	SDL_Window *win = nullptr;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			SDL_Log("Got param: %s", argv[i]);
			if (strcmpi(argv[i], "/s") == 0) {
				// Create an application window with the following settings:
				win = SDL_CreateWindow(
					"An SDL2 window",                  // window title
					SDL_WINDOWPOS_UNDEFINED,           // initial x position
					SDL_WINDOWPOS_UNDEFINED,           // initial y position
					640,                               // width, in pixels
					480,                               // height, in pixels
					SDL_WINDOW_FULLSCREEN_DESKTOP      // flags - see below
				);
			}
			else if (argv[i][0] == '/' && (argv[i][1] == 'c' || argv[i][1] == 'C')) {
				const SDL_MessageBoxButtonData buttons[] = {
					{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" }
				};
				const SDL_MessageBoxData messageboxdata = {
					SDL_MESSAGEBOX_INFORMATION, /* .flags */
					NULL, /* .window */
					"Configure", /* .title */
					"There is nothing you can configure.", /* .message */
					SDL_arraysize(buttons), /* .numbuttons */
					buttons, /* .buttons */
					NULL /* .colorScheme */
				};
				int buttonid;
				if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
					SDL_Log("error displaying message box");
					return 1;
				}
				return 0;
			}
			else if (argv[i][0] == '/' && (argv[i][1] == 'p' || argv[i][1] == 'P')) {
				int* winptr;
				sprintf(argv[i] + 3, "%d", &winptr);
				win = SDL_CreateWindowFrom(winptr);
			}
		}
	}

	if (win == nullptr) {
		std::cout << "Incorrect option or SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		system("pause");
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	int result = RunScreensaver(win, ren);

    SDL_Quit();
    return result;
}