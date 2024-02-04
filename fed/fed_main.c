#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "fed_buffer.h"

FedBuffer mainBuffer = { 0 };

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(
		"fed",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_INDEX8,
		SDL_TEXTUREACCESS_STATIC, 0, 0);

	bool running = true;
	while (running) {
		SDL_Event event = { 0 };
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: {
				running = false;
			} break;
			default:
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);
		
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}