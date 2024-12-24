/*
 * main.c
 *
 * Copyright 2024 Jani Juhani Sinervo <jani@sinervo.fi>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */

#include "SDL.h"

int
main(int argc, char **argv)
{
	int ret = EXIT_SUCCESS;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize windowing: %s", SDL_GetError());
		ret = EXIT_FAILURE;
		goto exit;
	}

	SDL_Window *wnd = SDL_CreateWindow(
	    "SDL Grab Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN
	);
	if (wnd == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Could not create fullscreen window: %s", SDL_GetError());
		ret = EXIT_FAILURE;
		goto exit;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(wnd, -1, 0);
	if (renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Could not create renderer: %s", SDL_GetError());
		ret = EXIT_FAILURE;
		goto exit;
	}

	SDL_bool should_quit = SDL_FALSE;
	while (!should_quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: {
				should_quit = SDL_TRUE;
				break;
			}
			case SDL_KEYUP: {
				SDL_KeyboardEvent *key_event = &event.key;
				if (key_event->keysym.sym == SDLK_ESCAPE) {
					should_quit = SDL_TRUE;
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				SDL_MouseButtonEvent *button_event = &event.button;
				if (button_event->button == 1) {
					SDL_bool grabbed = SDL_GetRelativeMouseMode();
					if (SDL_SetRelativeMouseMode(!grabbed) < 0) {
						SDL_LogCritical(
						    SDL_LOG_CATEGORY_APPLICATION,
						    "Could not set relative mouse mode: %s",
						    SDL_GetError()
						);
					} else {
						SDL_LogInfo(
						    SDL_LOG_CATEGORY_APPLICATION,
						    "Previous grab: \"%s\", current grab: \"%s\"",
						    grabbed ? "TRUE" : "FALSE",
						    grabbed ? "FALSE" : "TRUE"
						);
					}
				}
				break;
			}
			default:
				break;
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(wnd);
exit:
	SDL_Quit();
	return ret;
}

