#include "fed_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int buffer_init(Buffer* buf) {
	buf->lines = malloc(8 * sizeof(Line));
	if (!buf->lines) return -1;

	buf->capacity = 8;
	buf->size = 0;

	buf->filename = "*SCRATCH*";

	buf->column = 0;
	buf->line = 0;

	return 0;
}

int buffer_open_file(Buffer* buf, cstr path) {
	return 0;
}

static int line_init(Line* l) {
	l->text = malloc(8);
	if (!l->text) return -1;
	l->capacity = 8;
	l->size = 0;
	return 0;
}

static int line_free(Line* l) {
	free(l->text);
	return 0;
}

static int line_insert(Line* l, char c, u64 idx) {
	if (l->size >= l->capacity) {
		// Expande line buffer
		char* nline = malloc(l->capacity * 2);
		if (!nline) return -1;
		
		memmove(nline, l->text, l->size);

		l->capacity *= 2;

		free(l->text);

		l->text = nline;

		printf("[DBG] Expanding text buffer\n");
	}

	memmove(l->text + idx + 1, l->text + idx, l->size - idx);

	l->text[idx] = c;
	l->size++;

	return 0;
}

static int line_remove(Line* l, u64 idx) {
	memmove(l->text + idx, l->text + idx + 1, l->size - idx - 1);
	l->size--;
	return 0;
}

static int buffer_remove_line(Buffer* buf) {
	line_free(&buf->lines[buf->line]);
	memmove(buf->lines + buf->line, buf->lines + buf->line + 1, (buf->size - buf->line - 1) * sizeof(Line));
	buf->size--;
	return 0;
}

static int buffer_insert_line(Buffer* buf) {
	// Expand the buffer if there is no space left
	if (buf->size >= buf->capacity) {
		Line* nlines = malloc(buf->capacity * sizeof(Line) * 2);
		if (!nlines) return -1;

		memmove(nlines, buf->lines, buf->capacity * sizeof(Line) * 2);

		buf->capacity *= 2;

		free(buf->lines);

		buf->lines = nlines;
		printf("[DBG] Expanding lines buffer\n");
	}

	memmove(buf->lines + buf->line + 1, buf->lines + buf->line, (buf->size - buf->line)*sizeof(Line));

	line_init(&(buf->lines[buf->line]));
	buf->size++;

	return 0;
}

static int buffer_insert(Buffer* buf, char c) {
	if (buf->size == 0) {
		buffer_insert_line(buf);
	}

	if (c == '\n') {
		buf->line++;
		buffer_insert_line(buf);
		for (u64 i = buf->column; i < buf->lines[buf->line - 1].size; i++) {
			line_insert(&(buf->lines[buf->line]), buf->lines[buf->line - 1].text[i], i - buf->column);
		}
		buf->lines[buf->line - 1].size = buf->column;

		buf->column = 0;
		
		return 0;
	}

	if (line_insert(&(buf->lines[buf->line]), c, buf->column++)) return -1;
}

static int buffer_backspace(Buffer* buf) {

	if (!buf->column) {
		if (!buf->line) return 0;
		u64 lend = buf->lines[buf->line - 1].size;
		for (u64 i = 0; i < buf->lines[buf->line].size; i++) {
			line_insert(&buf->lines[buf->line - 1], buf->lines[buf->line].text[i], lend + i);
		}

		buffer_remove_line(buf);
		buf->line--;
		buf->column = lend;

		return 0;
	}

	line_remove(&buf->lines[buf->line], buf->column-1);
	buf->column--;
	return 0;
}

static int buffer_delete(Buffer* buf) {
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
			if (!buf->column) {
				if (buf->line) {
					buf->line--;
					buf->column = buf->lines[buf->line].size;
				}
			} else buf->column--;
		} break;
		case Right: {
			if (buf->column < buf->lines[buf->line].size && buf->size ) buf->column++;
			else if (buf->line < buf->size-1 && buf->size) {
				buf->column = 0;
				buf->line++;
			}
		} break;
		case Up: {
			
		} break;
		default:
			break;
		}
		printf("[DPG] (%d,%d)\n", buf->column, buf->line);
	} break;
	default:
		break;
	}

	return 0;
}
