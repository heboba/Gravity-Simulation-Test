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
bool CheckColisions(Object& object, Object& object2, Pos MoveVec = { 0,0 }, float n = 0);
bool CheckColisions(Pos pos, Object& object);
bool FindObject(Pos pos, Object** findedObject);
void WndResize(LPARAM lParam);
void FixedUpdate();
bool ResolveColisions(Entity& entity, Object& object, Pos VecMove);
Type CheckColSide(Entity& entity, Object& object, Pos VecMove);


bool IsParall(float a1, float a2, float b1, float b2);
Pos Intersect(Pos pos1, Pos pos2, Pos pos3, Pos pos4);
bool LineSegmentsIntersection(Pos pos1, Pos pos2, Pos pos3, Pos pos4);