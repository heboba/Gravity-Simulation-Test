#pragma once

//For Edit Mode
bool EditMode = true;
bool Moving = false;

//WndSize
int WndHeight;
int WndWight;

//Gravity and micro error
float g = 0.1;
float n = 0.00001;

//Pixels per block
float Size = 20;
float TextScale = 2;

//for text and textures
unsigned int textures[64];
float texCord[] = { 1,0, 0,0, 1,1, 0,1 };
float texCord2[] = { 0,0,0,1,1,1,1,0 };
float vertex[] = { 1,1,0,0,1,0,1,0,0,0,0,0 };
unsigned int TextTex;
float charWidthArray[256];