#include "fed_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int buffer_init(Buffer* buf) {
	buf->capacity = 8;
	buf->column = 0;
	buf->line = 0;
	buf->cursorPos = 0;
	buf->contents = malloc(8);
	if (!buf->contents) return -1;
	buf->filename = NULL;
	buf->size = 0;
}

int buffer_open_file(Buffer* buf, cstr path) {
	FILE* f = fopen(path, "r");
	if (!f) {
		fprintf(stderr, "Failed to open file: %s\n", path);
		return -1;
	}

	fseek(f, 0, SEEK_END);
	size_t fSize = ftell(f);
	rewind(f);

	char* contents = malloc(fSize);
	if (!contents) {
		fprintf(stderr, "Failed to allocate memory!\n");
		fclose(f);
		return -1;
	}

	buf->filename = path; // TODO(m1cha1s): Maybe we want to copy this string...
	buf->size = fSize;
	buf->contents = contents;
	buf->cursorPos = 0;

	fclose(f);
	
	return 0;
}

static int buffer_insert(Buffer* buf, char c) {
	// Expand the buffer if there is no space left
	if (buf->capacity <= buf->size) {
		char* nCont = malloc(buf->capacity * 2);
		if (!nCont) return -1;
		buf->capacity *= 2;
		memmove(nCont, buf->contents, buf->size);
		free(buf->contents);
		buf->contents = nCont;
		printf("[DBG] Expanding buffer...\n");
	}

	memmove(buf->contents + buf->cursorPos + 1, buf->contents + buf->cursorPos, buf->size - buf->cursorPos);

	buf->contents[buf->cursorPos++] = c;
	buf->size++;

}

static int buffer_backspace(Buffer* buf) {
	if (buf->size == 0 || buf->cursorPos == 0) return 0;

	memmove(buf->contents + buf->cursorPos - 1, buf->contents + buf->cursorPos, buf->size - buf->cursorPos);

	buf->size--;
	buf->cursorPos--;

	return 0;
}

static int buffer_delete(Buffer* buf) {
	if (buf->size == buf->cursorPos) return 0;

	memmove(buf->contents + buf->cursorPos, buf->contents + buf->cursorPos+1, buf->size - buf->cursorPos);

	buf->size--;
	//buf->cursorPos--;

	return 0;
}

int buffer_handle_event(Buffer* buf, Event ev)
{
	switch (ev.type) {
	case Keybaord: {
		char k = ev.kEvent.key;
		switch (k) {
		case '\b':
			buffer_backspace(buf);
			break;
		case 127:
			buffer_delete(buf);
			break;
		default:
			buffer_insert(buf, k);
			break;
		}
	} break;
	case Arrow: {
		switch (ev.aEvent) {
		case Left: {
			if (!buf->cursorPos) break;
			buf->cursorPos--;
		} break;
		case Right: {
			if (buf->cursorPos==buf->size) break;
			buf->cursorPos++;
		} break;
		default:
			break;
		}
	} break;
	default:
		break;
	}

	return 0;
}
