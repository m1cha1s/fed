#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "fed_platform.h"
#include "fed_common.h"
#include "fed_buffer.h"

/*

TODO:
- Check for potential SDL failures

*/

Buffer mainBuffer = { 0 };

int main() {
	buffer_init(&mainBuffer);

	platform_init();

	while (!platform_should_exit()) {
		Event e = platform_update();

		buffer_handle_event(&mainBuffer, e);

		platform_begin_drawing();

		platform_render_buffer(&mainBuffer);

		platform_end_drawing();
	}
    
	platform_deinit();
    
    return 0;
}