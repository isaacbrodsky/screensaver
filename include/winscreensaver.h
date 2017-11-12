/*
 * Windows screensaver / SDL specific startup
 */
#pragma once

int RunScreensaver(SDL_Window* win, SDL_Renderer* ren, void* testHwnd, bool closeOnMotion);