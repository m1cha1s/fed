#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "fed_common.h"

#include "fed_buffer.h"

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
						if (curpos) {
							curpos--;
							buffr[curpos] = '\0';
						}
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
			u64 line = 0;
			u64 column = 0;

			u64 xoff = 0;
			//u64 yoff = 0;

			// This is inefficient!!!
			for (u64 i = 0; i < strlen(buffr); i++) {
				if (buffr[i] == '\n') {
					line++;
					column = 0;
					xoff = 0;
					continue;
				}
				SDL_Surface* csurf = TTF_RenderGlyph_LCD(sans, buffr[i], white, (SDL_Color) { 0, 0, 0 });

				SDL_Texture* ctext = SDL_CreateTextureFromSurface(renderer, csurf);
				SDL_Rect messageRect = {
					.x = xoff,
					.y = line * csurf->h,
					.w = csurf->w,
					.h = csurf->h
				};

				column++;
				xoff += csurf->w;

				SDL_RenderCopy(renderer, ctext, NULL, &messageRect);

				SDL_DestroyTexture(ctext);
				SDL_FreeSurface(csurf);
			}
		}

		SDL_RenderPresent(renderer);
	}
    
	TTF_CloseFont(sans);
	TTF_Quit();

	SDL_Quit();
    
    return 0;
}