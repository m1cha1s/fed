#ifndef _fed_buffer_h
#define _fed_buffer_h

#include "fed_common.h"
#include "fed_event.h"

typedef struct {
	char *text;
	u64 size;
	u64 capacity;
} Line;

typedef struct {
	// This stores the whole text buffer
	Line *lines;
	u64 size;
	u64 capacity;

	// This stores the opened files filename
	cstr filename;

	// This is the cursor data
	u64 column;
	u64 line;

	u64 max_column;
} Buffer;

int buffer_init(Buffer* buf);
int buffer_open_file(Buffer *buf, cstr path);
int buffer_handle_event(Buffer* buf, Event ev);

#endif