#pragma once
#include <windows.h>
#include <filesystem>
#include <iostream>

void RedirectIOToConsole()
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    std::cout.clear();
}
void WndResize(LPARAM lParam) {
    glLoadIdentity();
    glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
    glOrtho(0, LOWORD(lParam) / 10, 0, HIWORD(lParam) / 10, 0, 1);
}