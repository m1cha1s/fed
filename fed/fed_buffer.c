#include "fed_buffer.h"

#include <stdio.h>
#include <stdlib.h>

int FedBufferOpenFile(FedBuffer* buf, char* path) {
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
	buf->bufferSize = fSize;
	buf->contents = contents;
	buf->cursorPos = 0;

	fclose(f);
	
	return 0;
}

int FedBufferDraw(FedBuffer* buf) {
	return 0;
}