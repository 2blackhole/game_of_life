
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "logic.h"
#include "constants.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("game of life");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR     lpCmdLine,
   _In_ int       nCmdShow
)
{
   WNDCLASSEX wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
   wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName   = NULL;
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

   if (!RegisterClassEx(&wcex))
   {
      MessageBox(NULL,
         _T("Call to RegisterClassEx failed!"),
         _T("Windows Desktop Guided Tour"),
         NULL);

      return 1;
   }

   // Store instance handle in our global variable
   hInst = hInstance;

   // The parameters to CreateWindowEx explained:
   // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
   // szWindowClass: the name of the application
   // szTitle: the text that appears in the title bar
   // WS_OVERLAPPEDWINDOW: the type of window to create
   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
   // 500, 100: initial size (width, length)
   // NULL: the parent of this window
   // NULL: this application does not have a menu bar
   // hInstance: the first parameter from WinMain
   // NULL: not used in this application
   HWND hWnd = CreateWindowEx(
      WS_EX_OVERLAPPEDWINDOW,
      szWindowClass,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      SCREEN_WIDTH, SCREEN_HEIGHT,
      NULL,
      NULL,
      hInstance,
      NULL
   );

   if (!hWnd)
   {
      MessageBox(NULL,
         _T("Call to CreateWindow failed!"),
         _T("Windows Desktop Guided Tour"),
         NULL);

      return 1;
   }

   // The parameters to ShowWindow explained:
   // hWnd: the value returned from CreateWindow
   // nCmdShow: the fourth parameter from WinMain
   ShowWindow(hWnd,
      nCmdShow);
   UpdateWindow(hWnd);

   // Main message loop:
   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return (int) msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   PAINTSTRUCT ps;
   HDC hdc;

   int ** grid = init_grid(GRID_X, GRID_Y);

   grid[20][20] = 1;
   grid[21][20] = 1;
   grid[19][20] = 1;
   grid[20][19] = 1;
   grid[20][18] = 1;
   grid[20][17] = 1;

   printf("banana\n");


   switch (message)
   {
   case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);

      HPEN hPen = CreatePen(PS_SOLID, 1, RGB(69, 69, 69));
      HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

      HBRUSH hBGBrush = CreateSolidBrush(RGB(0, 0, 0));
      RECT clientRect;
      GetClientRect(hWnd, &clientRect);
      FillRect(hdc, &clientRect, hBGBrush);
      DeleteObject(hBGBrush);



      //Отрисовка клеток
      for(int x = 0; x < GRID_X; x++) {
         for(int y = 0; y < GRID_Y; y++) {
            if (grid[x][y]) {
               RECT cellRect = { x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE };
               HBRUSH hBrush = CreateSolidBrush(RGB(138, 43, 226));
               FillRect(hdc, &cellRect, hBrush);
               DeleteObject(hBrush);
            }
         }
      }
      //Отрисовка клеток конец

      //сетка
      for(int x = 0; x <= SCREEN_WIDTH; x += CELL_SIZE) {
         MoveToEx(hdc, x, 0, NULL);
         LineTo(hdc, x, SCREEN_HEIGHT);
      }

      for(int y = 0; y <= SCREEN_HEIGHT; y += CELL_SIZE) {
         MoveToEx(hdc, 0, y, NULL);
         LineTo(hdc, SCREEN_WIDTH, y);
      }
      //сетка конец

      SelectObject(hdc, hOldPen);
      DeleteObject(hPen);
      //комент норм
      // TextOut(hdc,
      //    5, 5,
      //    greeting, _tcslen(greeting));
      //комент норм конец

      EndPaint(hWnd, &ps);
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
      break;
   }


   free_grid(grid);
   return 0;
}