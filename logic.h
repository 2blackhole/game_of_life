#pragma once
#include "constants.h"

int game_loop();

int render();

int draw(int ** grid);

int ** init_grid(int width, int height);

void free_grid(int ** grid);