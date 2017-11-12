/*
 * Windows specific parts, including entry point.
 */
#include <iostream>
#include "SDL.h"
#include "SDL_syswm.h"
#include "winscreensaver.h"
#include "main.h"

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Receive window manager events so we can close from preview
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	SDL_Window *win = nullptr;
	void *winptr = nullptr;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			SDL_Log("Got param: %s", argv[i]);
			if (strcmpi(argv[i], "/debug") == 0) {
				// Create an application window with the following settings:
				win = SDL_CreateWindow(
					"An SDL2 window",                  // window title
					SDL_WINDOWPOS_UNDEFINED,           // initial x position
					SDL_WINDOWPOS_UNDEFINED,           // initial y position
					1024,                              // width, in pixels
					768,                               // height, in pixels
					0                                  // flags - see below
				);
			}
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
				char *chptr = argv[i] + 2;
				if (*chptr == '\0') {
					i++;
					chptr = argv[i];
				}
				winptr = (void*)atoll(chptr);
				SDL_Log("Embedding in window: %u", winptr);
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

	int result = RunScreensaver(win, ren, winptr);

	SDL_Quit();
	return result;
}

int RunScreensaver(SDL_Window* win, SDL_Renderer* ren, void* testHwnd) {
#if _WINDOWS
	SDL_assert(sizeof(void*) == sizeof(HWND));
#endif

	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	int i = 0;
	SDL_Log("Detected window size: %d %d", w, h);

	Scrsvr_State state(ren, w, h, 4);

	Uint32 lastTime = SDL_GetTicks();

	bool keepRunning = true;
	while (keepRunning) {
#if _WINDOWS
		if (testHwnd != nullptr) {
			// Hack to detect switching away from the preview in the screensaver control panel:
			// https://stackoverflow.com/questions/37569309/how-to-finish-sdl-screensaver-app-after-preview-window-closes
			if (!IsWindow((HWND)testHwnd)) {
				SDL_Log("Closing because testHwnd is not a window");
				keepRunning = false;
			}
		}
#endif

		// Handle input/events
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
			case SDL_WINDOWEVENT_CLOSE:
				keepRunning = false;
				break;
#if _WINDOWS
			case SDL_SYSWMEVENT:
				if (event.syswm.msg->msg.win.msg == 0xF140 /*SCREENSAVE*/ || event.syswm.msg->msg.win.msg == 0xF060 /* SC_CLOSE */)
					keepRunning = false;
				break;
#endif
			}
			SDL_Log("Event: %u", event.type);
		}

		// Calculate elapsed time
		Uint32 currentTime = SDL_GetTicks();
		Uint32 elapsedTime = 0;
		if (currentTime < lastTime) {
			// Truncate part of the update
			elapsedTime = currentTime;
		}
		else {
			elapsedTime = lastTime - currentTime;
		}
		lastTime = currentTime;

		// Update
		state.Update(elapsedTime);

		// Draw
		SDL_SetRenderTarget(ren, NULL);
		state.Draw(ren);
		SDL_RenderPresent(ren);
		SDL_Delay(1);
	}

	return 0;
}