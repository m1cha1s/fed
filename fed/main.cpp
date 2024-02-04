#include <iostream>

extern "C" {
#include "raylib.h"
}

int main() {
	InitWindow(800, 800, "fed");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		
		ClearBackground(BLACK);
		DrawText("Balls", 10, 10, 50, WHITE);

		EndDrawing();
	}
	return 0;
}