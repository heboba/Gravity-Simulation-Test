#pragma once
#include "Resources.h"

HINSTANCE hInstance;
HINSTANCE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;

WNDCLASSEX wcex;
HWND hwnd;
HDC hDC;
HGLRC hRC;
MSG msg;
BOOL bQuit = FALSE;


microseconds Now;
Player* player;
Object obj;
Object obj2;
Object obj3;
Object* Active;

Pos MouseRelativeCoordinates;

EditActionType EditType = None;



vector <Object*> DrawList;