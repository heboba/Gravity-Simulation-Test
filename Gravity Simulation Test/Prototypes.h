#pragma once
#include "Structs.h"
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void InitWindow();
void MainLoop();
void Init();
void Draw(Object& object);
void CalculatePhisic(Entity& entity);
bool CheckColisions(Object& object, Object& object2);
void WndResize(LPARAM lParam);
void FixedUpdate();