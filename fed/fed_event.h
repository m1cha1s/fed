#ifndef _fed_event_t
#define _fed_event_t

#include <stdbool.h>

#include "fed_common.h"

typedef enum {
	NoEvent,
	Keybaord,
	Arrow,
	Special,
} EventType;

typedef enum {
	Up, Down, Left, Right
} ArrowEvent;

typedef enum {
	PageUp, PageDown, Home, End, Escape, Tab,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
} SpecialEvent;

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
	SpecialEvent sEvent;
} Event;

#endif