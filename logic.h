#pragma once

#include <windows.h>

typedef enum {
   DEAD = 0,
   ALIVE = 1
} Cell;

typedef enum {
   END_PLS = 0,
   INSERT_MODE = 1,
   RUNNING_MODE = 2
} game_state;

void render(int * grid, HWND hwnd);

void draw(const int * grid, HWND * hwnd);

void game_input(game_state * g_state, HWND hwnd);
