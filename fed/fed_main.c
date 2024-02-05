#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "fed_common.h"

#include "fed_buffer.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

/*

TODO:
- Check for potential SDL failures

*/

FedBuffer mainBuffer = { 0 };


int main() {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_Window* window = SDL_CreateWindow("fed",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
	// Font test stuff
	TTF_Font* sans = TTF_OpenFont("c:/windows/fonts/calibri.ttf", 20);
	SDL_Color white = { 255, 255, 255 };

	char buffr[1024] = { 0 };
	u64 curpos = 0;

	SDL_StartTextInput();

	bool running = true;
	while (running) {
		SDL_Event event = { 0 };
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
                case SDL_QUIT: {
                    running = false;
                } break;
				case SDL_TEXTINPUT: {
					buffr[curpos++] = event.text.text[0];
				} break;
				case SDL_KEYDOWN: {
					switch (event.key.keysym.sym)
					{
					case SDLK_BACKSPACE: {
						curpos--;
						buffr[curpos] = '\0';
					} break;
					case SDLK_RETURN: {
						buffr[curpos++] = '\n';
					} break;
					default:
						break;
					}
				} break;
                default:
				break;
			}
		}
        
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		SDL_RenderClear(renderer);

		if (buffr[0]) {
			SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, buffr, white);
			SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
			SDL_Rect messageRect = {
				.x = 0,
				.y = 0,
				.w = surfaceMessage->w,
				.h = surfaceMessage->h
			};


			SDL_RenderCopy(renderer, message, NULL, &messageRect);

			SDL_DestroyTexture(message);
			SDL_FreeSurface(surfaceMessage);
		}

		SDL_RenderPresent(renderer);
	}
    
	TTF_CloseFont(sans);
	TTF_Quit();

	SDL_Quit();
    
    return 0;
}