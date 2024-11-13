#include <stdio.h>
#include <windows.h>

#include "logic.h"
#include "constants.h"

void render(int *grid) {
   int newGrid[GRID_X][GRID_Y];

   for (int y = 0; y < GRID_Y; ++y) {
      for (int x = 0; x < GRID_X; ++x) {
         int aliveNeighbors = 0;

         for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
               if (i == 0 && j == 0) continue;
               int newX = x + j;
               int newY = y + i;
               if (newX >= 0 && newX < GRID_X && newY >= 0 && newY < GRID_Y) {
                  if (grid[newY * GRID_X + newX]) {
                     ++aliveNeighbors;
                  }
               }
            }
         }

         if (grid[y * GRID_X + x]) {
            if (aliveNeighbors == 2 || aliveNeighbors == 3) {
               newGrid[y][x] = 1;
            } else {
               newGrid[y][x] = 0;
            }
         } else {
            if (aliveNeighbors == 3) {
               newGrid[y][x] = 1;
            } else {
               newGrid[y][x] = 0;
            }
         }
      }
   }

   for (int y = 0; y < GRID_Y; ++y) {
      for (int x = 0; x < GRID_X; ++x) {
         grid[y * GRID_X + x] = newGrid[y][x];
      }
   }
}


void draw(const int * grid, const HDC * hdc) {
   for(int y = 0; y < GRID_X; y++) {
      for(int x = 0; x < GRID_Y; x++) {
         if (grid[x * GRID_Y + y]) {
            RECT cellRect = { x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE };
            HBRUSH hBrush = CreateSolidBrush(RGB(138, 43, 226));
            FillRect(*hdc, &cellRect, hBrush);
            DeleteObject(hBrush);
         }
      }
   }


   for(int x = 0; x <= SCREEN_WIDTH; x += CELL_SIZE) {
      MoveToEx(*hdc, x, 0, NULL);
      LineTo(*hdc, x, SCREEN_HEIGHT);
   }

   for(int y = 0; y <= SCREEN_HEIGHT; y += CELL_SIZE) {
      MoveToEx(*hdc, 0, y, NULL);
      LineTo(*hdc, SCREEN_WIDTH, y);
   }
}

void game_input(game_state * g_state, HWND hwnd) {

   if (GetKeyState(VK_SPACE) == 1) {
      *g_state = RUNNING_MODE;
   }
   if (GetKeyState(VK_SPACE) == 0) {
      *g_state = INSERT_MODE;
   }

}
