
#include <stdint.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "logic.h"
#include "constants.h"

// Global variables
int grid[GRID_X][GRID_Y] = {0};
game_state g_state = INSERT_MODE;

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


   int64_t frame_start = 0;
   int64_t frame_end = 0;
   int64_t accumulator = 0;

   while (g_state != END_PLS) {

      //QueryPerformanceCounter((LARGE_INTEGER*)&frame_start);
      while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
         DispatchMessage(&msg);
         TranslateMessage(&msg);
      }

      //ввод с клавиатуры, рендер игры
      game_input(&g_state, hWnd);

      if (g_state == RUNNING_MODE) {
         Sleep(200);
         render(grid);
      }

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
   
   switch (message) {
      case WM_LBUTTONDOWN:
         if (g_state != INSERT_MODE) {break;}
         int x = LOWORD(lParam) / CELL_SIZE;
         int y = HIWORD(lParam) / CELL_SIZE;
         if (x >= 0 && x < GRID_X && y >= 0 && y < GRID_Y) {
            grid[x][y] = !grid[x][y]; //переключение состояния
            InvalidateRect(hWnd, NULL, FALSE);
         }
      case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);

      HPEN hPen = CreatePen(PS_SOLID, 1, RGB(69, 69, 69));
      HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

      HBRUSH hBGBrush = CreateSolidBrush(RGB(0, 0, 0));
      RECT clientRect;
      GetClientRect(hWnd, &clientRect);
      FillRect(hdc, &clientRect, hBGBrush);
      DeleteObject(hBGBrush);


      draw(grid, &hdc);


      SelectObject(hdc, hOldPen);
      DeleteObject(hPen);

      EndPaint(hWnd, &ps);
      break;
      case WM_DESTROY:
         PostQuitMessage(0);
      break;
      case WM_CLOSE:
         g_state = END_PLS;
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
      break;
   }

   return 0;
}
