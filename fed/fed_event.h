#ifndef _fed_event_t
#define _fed_event_t

#include <stdbool.h>

#include "fed_common.h"

typedef enum {
	NoEvent,
	Keybaord,
	Arrow,
} EventType;

typedef enum {
	Up, Down, Left, Right
} ArrowEvent;

typedef struct {
	bool CTRL;
	bool SHIFT;
	bool ALT;
	char key;
} KeyboardEvent;

typedef struct {
	EventType type;
	KeyboardEvent kEvent;
	ArrowEvent aEvent;
} Event;

#endif