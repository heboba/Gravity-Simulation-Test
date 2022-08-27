#pragma once
#include "Structs.h"
#include <Windows.h>

using namespace std;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
Pos GetMousePos(LPARAM lParam);
void InitWindow();
void MainLoop();
void Init();
void Draw(Object& object, Pos& Camera);
void CalculatePhisic(Entity& entity);
bool CheckColisions(Object& object, Object& object2, Pos MoveVec = { 0,0 }, float n = 0);
bool CheckColisions(Pos pos, Object& object);
bool FindObject(Pos pos, Object** findedObject);
void WndResize();
void WndResize(LPARAM lParam);
void FixedUpdate();
void Update();
bool ResolveColisions(Entity& entity, Object& object, Pos VecMove);
Type CheckColSide(Entity& entity, Object& object, Pos VecMove);

void SaveGame(string);
void LoadGame(string);


bool IsParall(float a1, float a2, float b1, float b2);
Pos Intersect(Pos pos1, Pos pos2, Pos pos3, Pos pos4);
bool LineSegmentsIntersection(Pos pos1, Pos pos2, Pos pos3, Pos pos4);

void SetCharSize(unsigned char* data, int widht, int cnt, float* cWidthArray, int checkByte);
void LoadTexture(string filename, unsigned int* target);
void TexturesInit();
void DrawText(string str, float x, float y, float Scale, float limit = 999);
void Drawv2();

