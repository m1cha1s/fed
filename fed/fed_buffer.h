#ifndef _fed_buffer_h
#define _fed_buffer_h

#include <stddef.h>

typedef struct {
	char *contents;
	char *filename;
	size_t bufferSize;
	size_t cursorPos;
} FedBuffer;

int FedBufferOpenFile(FedBuffer *buf, char *path);
int FedBufferDraw(FedBuffer *buf);

#endif