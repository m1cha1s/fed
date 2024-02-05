#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "fed_buffer.h"

/*

TODO:
- Check for potential SDL failures

*/

FedBuffer mainBuffer = { 0 };

char ttf_buffer[1 << 25];

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_Window* window = SDL_CreateWindow("fed",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
	// Font test stuff
	TTF_Font* sans = TTF_OpenFont("c:/windows/fonts/calibri.ttf", 128);
	SDL_Color white = { 255, 255, 255 };

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, "Balls", white);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Rect messageRect = { 
		.x=0,
		.y=0,
		.w=surfaceMessage->w,
		.h=surfaceMessage->h
	};

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

		SDL_RenderCopy(renderer, message, NULL, &messageRect);
        		
		SDL_RenderPresent(renderer);
	}
    
	SDL_Quit();
    
    return 0;
}