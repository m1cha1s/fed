#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

#include "fed_buffer.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

/*

TODO:
- Check for potential SDL failures

*/

FedBuffer mainBuffer = { 0 };

char ttf_buffer[1 << 25];

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

	stbtt_fontinfo font;
	unsigned char* bitmap;
	
	fread(ttf_buffer, 1, 1 << 25, fopen("c:/windows/fonts/arialbd.ttf", "rb"));

	int w, h;

	stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
	bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, 20), 'G', &w, &h, 0, 0);

	SDL_Texture* atlas = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STATIC,
		w, 
		h
	);

	Uint32* pixels = malloc(w * h * sizeof(Uint32));
	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
	for (int i = 0; i < w * h; i++) {
		pixels[i] = SDL_MapRGBA(format, 0xff, 0xff, 0xff, bitmap[i]);
	}
	SDL_UpdateTexture(atlas, NULL, pixels, w * sizeof(Uint32));
	free(pixels);

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

		SDL_RenderCopy(renderer, atlas, NULL, atlas);
		
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}