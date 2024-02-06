#if 1

#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "fed_platform.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;
int fontSize = 20;
SDL_Color foreground = { 255,255,255,255 };
SDL_Color background = { 0,0,0,255 };

bool shouldExit = false;

// Initialize the window
int platform_init(void) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	window = SDL_CreateWindow("fed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	font = TTF_OpenFont(DEFAULT_FONT, fontSize);

	SDL_StartTextInput();
}

// Deinitialize the window and platform dependent stuff
int platform_deinit(void) {
	TTF_CloseFont(font);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool CTRL = false;
bool SHIFT = false;
bool ALT = false;

bool up = false;
bool down = false;
bool left = false;
bool right = false;

// Update (input handling and other stuff)
Event platform_update(void) {
	SDL_Event event = { 0 };
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: {
			shouldExit = true;
		} break;
		case SDL_TEXTINPUT: {
			char key = event.text.text[0];
			return (Event) { 
				.type = Keybaord, 
				.kEvent = { 
					.CTRL = CTRL, 
					.SHIFT = SHIFT, 
					.ALT = ALT, 
					.key = key 
				} 
			};
		} break;
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE: return (Event) {
				.type = Keybaord,
					.kEvent = {
					.CTRL = CTRL,
					.SHIFT = SHIFT,
					.ALT = ALT,
					.key = '\b'
				}
			};
			case SDLK_RETURN: return (Event) {
				.type = Keybaord,
					.kEvent = {
						.CTRL = CTRL,
						.SHIFT = SHIFT,
						.ALT = ALT,
						.key = '\n'
				}
			};
			case SDLK_LCTRL:
			case SDLK_RCTRL: {
				CTRL = true;
			} break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT: {
				SHIFT = true;
			} break;
			case SDLK_LALT:
			case SDLK_RALT: {
				ALT = true;
			} break;
			case SDLK_UP: return (Event) {
				.type = Arrow,
				.aEvent = Up,
			};
			case SDLK_DOWN: return (Event) {
				.type = Arrow,
				.aEvent = Down,
			};
			case SDLK_LEFT: return (Event) {
				.type = Arrow,
				.aEvent = Left,
			};
			case SDLK_RIGHT: return (Event) {
				.type = Arrow,
				.aEvent = Right,
			};
			/*case SDLK_UP: {
				up = true;
			} break;
			case SDLK_DOWN: {
				down = true;
			} break;
			case SDLK_LEFT: {
				left = true;
			} break;
			case SDLK_RIGHT: {
				right = true;
			} break;*/
			default:
				break;
			}
		} break;
		case SDL_KEYUP: {
			switch (event.key.keysym.sym)
			{
			case SDLK_BACKSPACE: {

			} break;
			case SDLK_RETURN: {

			} break;
			case SDLK_LCTRL:
			case SDLK_RCTRL: {
				CTRL = false;
			} break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT: {
				SHIFT = false;
			} break;
			case SDLK_LALT:
			case SDLK_RALT: {
				ALT = false;
			} break;
			/*case SDLK_UP: {
				up = false;
			} break;
			case SDLK_DOWN: {
				down = false;
			} break;
			case SDLK_LEFT: {
				left = false;
			} break;
			case SDLK_RIGHT: {
				right = false;
			} break;*/
			default:
				break;
			}
		} break;
		default:
			break;
		}
	}

	return (Event) { .type = NoEvent };
}

// This function is to be used inside a while(!*this func*) to test if we should quit.
int platform_should_exit(void) {
	return shouldExit;
}


int platform_begin_drawing(void) {
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderClear(renderer);
}

int platform_end_drawing(void) {
	SDL_RenderPresent(renderer);
}

// Render the buffer to the screen
int platform_render_buffer(Buffer* b) {
	u64 line = 0;
	u64 column = 0;
	u64 xoff = 0;

	u32 ch = 0;

	for (u64 i = 0; i <= b->size; ++i) {
		bool cnow = i == b->cursorPos;
		bool clast = b->cursorPos == b->size;

		if (i==b->size && !clast) return 0;

		if (i == b->size && !clast);
		if (b->contents[i] == '\n') {
			line++;
			column = 0;
			xoff = 0;
			continue;
		}

		SDL_Surface* csur = TTF_RenderGlyph_LCD(font, cnow && clast ? ' ' : b->contents[i], cnow ? background : foreground, cnow ? foreground : background);
		SDL_Texture* ctex = SDL_CreateTextureFromSurface(renderer, csur);

		SDL_Rect cRect = {
			.x = xoff,
			.y = line * csur->h,
			.w = csur->w,
			.h = csur->h,
		};

		ch = csur->h;

		xoff += csur->w;
		column++;

		SDL_RenderCopy(renderer, ctex, NULL, &cRect);

		SDL_FreeSurface(csur);
		SDL_DestroyTexture(ctex);
	}

	return 0;
}

#endif