#ifndef _fed_platform_h
#define _fed_platform_h

#include <stdbool.h>

#include "fed_buffer.h"
#include "fed_event.h"

#define DEFAULT_FONT "c:/windows/fonts/CascadiaCode.ttf"

int platform_init(void); // Initialize the window
int platform_deinit(void); // Deinitialize the window and platform dependent stuff
Event platform_update(void); // Update (input handling and other stuff)
int platform_should_exit(void); // This function is to be used inside a while(!*this func*) to test if we should quit.

int platform_begin_drawing(void);
int platform_end_drawing(void);

int platform_render_buffer(Buffer* b); // Render the buffer to the screen

#endif