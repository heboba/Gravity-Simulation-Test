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

bool IsParall(float a1, float a2, float b1, float b2)
{
    if ((a1 / a2) == (b1 / b2))
    {
        return false;
    }
    return true;
}
Pos Intersect(Pos pos1, Pos pos2, Pos pos3, Pos pos4)
{

    float a1 = pos1.y - pos2.y;
    float b1 = pos2.x - pos1.x;
    float c1 = pos1.x * pos2.y - pos2.x * pos1.y;
    float a2 = pos3.y - pos4.y;
    float b2 = pos4.x - pos3.x;
    float c2 = pos3.x * pos4.y - pos4.x * pos3.y;


    double det = a1 * b2 - a2 * b1;
    float x = (b1 * c2 - b2 * c1) / det;
    float y = (a2 * c1 - a1 * c2) / det;
    return { x,y };
}
bool LineSegmentsIntersection(Pos pos1, Pos pos2, Pos pos3, Pos pos4)
{
    double _1 = pos1.x - pos2.x;
    double _2 = pos1.x - pos3.x;
    double _3 = pos3.x - pos4.x;
    double _4 = pos1.y - pos2.y;
    double _5 = pos1.y - pos3.y;
    double _6 = pos3.y - pos4.y;
    double d = _1 * _6 - _4 * _3;
    double t = (_2 * _6 - _5 * _3) / d;
    double u = (_2 * _4 - _5 * _1) / d;
    return t >= 0. && t <= 1. && u >= 0. && u <= 1.;
}