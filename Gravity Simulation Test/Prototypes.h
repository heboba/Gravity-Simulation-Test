#pragma once
#include "Structs.h"
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void InitWindow();
void MainLoop();
void Init();
void CalculatePhisic(Object& object);
void WndResize(LPARAM lParam);