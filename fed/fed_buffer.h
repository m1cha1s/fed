#ifndef _fed_buffer_h
#define _fed_buffer_h

#include "fed_common.h"
#include "fed_event.h"

typedef struct {
	char *contents;
	cstr filename;
	u64 size;
	u64 capacity;
	u64 cursorPos;
	u64 column;
	u64 line;
} Buffer;

int buffer_init(Buffer* buf);
int buffer_open_file(Buffer *buf, cstr path);
int buffer_handle_event(Buffer* buf, Event ev);

#endif