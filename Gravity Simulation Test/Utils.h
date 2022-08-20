#pragma once
#include <windows.h>
#include <filesystem>
#include <iostream>
#include "Varibles.h"

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

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
    WndHeight = HIWORD(lParam);
    WndWight = LOWORD(lParam);
    glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
    glOrtho(0, LOWORD(lParam) / Size, 0, HIWORD(lParam) / Size, 0, 1);
}
Pos GetMousePos(LPARAM lParam) {
    return { float(LOWORD(lParam) / Size), float((WndHeight - HIWORD(lParam)) / Size) };
};

//Clever unused Stuff
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

//For Colisions
float GetDist(float cord1, float cord2, float cord3, float cord4)
{
    if (cord1 > cord3 + cord4) return cord1 - (cord3 + cord4);
    if (cord1 + cord2 < cord3) return cord3 - (cord1 + cord2);
    //return cord1 - cord3;
    return 0;
}
bool ResolveColisions(Entity& entity, Object& object, Pos VecMove) {

    if (CheckColisions(entity, object)) {
        while (CheckColisions(entity,object))
        {
            entity.pos.y += 1;
        }
        return true;
    }



    Type t = CheckColSide(entity, object, VecMove);
    float d = 0, d2 = 0, h = 0, h2 = 0, kof = 0;

    switch (t)
    {
    case Top:
        h = GetDist(entity.pos.y, entity.pos2.y, object.pos.y, object.pos2.y);
        h2 = entity.pos.y + entity.pos2.y - (entity.pos.y + entity.pos2.y + entity.MoveVec.y);

        kof = (1 - h / h2) + 0.0001;
        entity.MoveVec = { entity.MoveVec.x - entity.MoveVec.x * kof,entity.MoveVec.y - entity.MoveVec.y * kof };
        return true;
    case Bottom:
        h = GetDist(entity.pos.y, entity.pos2.y, object.pos.y, object.pos2.y);
        h2 = entity.pos.y + entity.pos2.y + entity.MoveVec.y - (entity.pos.y + entity.pos2.y);

        kof = (1 - h / h2) + 0.0001;
        entity.MoveVec = { entity.MoveVec.x - entity.MoveVec.x * kof,entity.MoveVec.y - entity.MoveVec.y * kof };
        return true;
    case Left:
        d = GetDist(entity.pos.x, entity.pos2.x, object.pos.x, object.pos2.x);
        d2 = entity.pos.x + entity.pos2.x + entity.MoveVec.x - (entity.pos.x + entity.pos2.x);

        kof = (1 - d / d2) + 0.0001;
        entity.MoveVec = { entity.MoveVec.x - entity.MoveVec.x * kof,entity.MoveVec.y};
        return false;
    case Right:
        d = GetDist(entity.pos.x, entity.pos2.x, object.pos.x, object.pos2.x);
        d2 = entity.pos.x + entity.pos2.x - (entity.pos.x + entity.pos2.x + entity.MoveVec.x);

        kof = (1 - d / d2) + 0.0001;
        entity.MoveVec = { entity.MoveVec.x - entity.MoveVec.x * kof,entity.MoveVec.y };
        return false;
    }
}
Type CheckColSide(Entity &entity, Object &object, Pos VecMove) {
    float h = GetDist(entity.pos.y, entity.pos2.y, object.pos.y, object.pos2.y);
    float d = GetDist(entity.pos.x, entity.pos2.x, object.pos.x, object.pos2.x);

    if (h == 0 || entity.MoveVec.y == 0) return VecMove.x > 0 ? Left : Right;
    if (d == 0 || entity.MoveVec.x == 0) return VecMove.y < 0 ? Top : Bottom;
    if (h / entity.MoveVec.x > d / entity.MoveVec.y) return VecMove.y < 0 ? Top : Bottom;
    if (h / entity.MoveVec.x <= d / entity.MoveVec.y) return VecMove.x > 0 ? Left : Right;

    return (Type)404;
}
bool FindObject(Pos pos, Object** findedObject) {
    for (auto i : DrawList) {
        if (CheckColisions(pos, *i)) {
            *findedObject = i;
            return true;
        }
    }
    return false;
}

//Colisions Check
bool CheckColisions(Object& object, Object& object2, Pos MoveVec, float n) {
    Pos nextPos = object.pos + MoveVec;
    float xA[2] = { nextPos.x,nextPos.x + object.pos2.x };
    float xB[2] = { object2.pos.x ,object2.pos.x + object2.pos2.x };

    float yA[2] = { nextPos.y + n,nextPos.y + object.pos2.y };
    float yB[2] = { object2.pos.y ,object2.pos.y + object2.pos2.y };

    if (xA[1] < xB[0] || yA[1] < yB[0] || yA[0] > yB[1] || xA[0] > xB[1]) return false;
    return true;
}
bool CheckColisions(Pos pos, Object& object) {
    float xB[2] = { object.pos.x ,object.pos.x + object.pos2.x };
    float yB[2] = { object.pos.y ,object.pos.y + object.pos2.y };

    if (pos.x < xB[0] || pos.y < yB[0] || pos.y > yB[1] || pos.x > xB[1]) return false;
    return true;
}

//For Phisic
void CalculatePhisic(Entity& entity) {
    entity.pos = entity.pos + entity.MoveVec;
    if (entity.pos.y < 0) {
        entity.pos.y = 0;
        entity.MoveVec.y = 0;
    }
    if (entity.pos.y > 0 && !player.OnGround && !Moving) {
        entity.MoveVec.y = entity.MoveVec.y - g;
    }
}

//Work with OpenGl
void Draw(Object& object) {
    glPushMatrix();
    glColor3f(object.color[0], object.color[1], object.color[2]);

    glTranslatef(object.pos.x, object.pos.y, 0);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0, 0);
    glVertex2f(0, object.pos2.y);
    glVertex2f(object.pos2.x, object.pos2.y);
    glVertex2f(object.pos2.x, 0);

    glEnd();

    glPopMatrix();
}