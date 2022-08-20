#pragma once

#include <iostream>

enum Type {
    Top = 1, Left = 0, Right = 2, Bottom = 3, Error = 404
};

struct Pos {
    float x, y;
    Pos operator+(Pos pos2) {
        return  { x + pos2.x,y + pos2.y };
    }
    Pos operator-(Pos pos2) {
        return  { x - pos2.x,y - pos2.y };
    }
    Pos operator=(Pos pos2) {
        x = pos2.x;
        y = pos2.y;
        return *this;
    }
};
class Object {
public:
    Pos pos;
    Pos pos2;
    float color[3] = { 1,1,1 };
    Object(float x1, float y1, float x2, float y2) {
        pos = { x1,y1 };
        pos2 = { x2,y2 };
    }
    Object() {
        pos = { 0,0 };
        pos2 = { 0,0 };
    }
};
class Entity : public Object{
public:
    float restitution;
    bool OnGround = true;
    Pos MoveVec;
    void Move() {
        pos = pos + MoveVec;
        if (pos.y < 0) {
            pos.y = 0;
            MoveVec.y = 0;
        }
    }
    void Jump() {
        MoveVec.y = 2;
    }
};
class Player : public Entity {
public:
    int JumpCnt;
    bool JumpPressed = false;
    void Move() {
        this->MoveVec.x = 0;
        if ((GetKeyState(65) & 0x8000) != 0) this->MoveVec.x -= 0.5;
        if ((GetKeyState(68) & 0x8000) != 0) this->MoveVec.x += 0.5;
        if ((GetKeyState(32) & 0x8000) != 0) {
            if (pos.y == 0 || OnGround)
            {
                this->Jump();
                JumpCnt = 0;
                JumpPressed = true;
                std::cout << "First Jump\n";
            }
            else if (JumpCnt != 1 && !JumpPressed) {
                this->Jump();
                JumpCnt = 1;
                std::cout << "Second Jump\n";
            }
        }
        else
        {
            JumpPressed = false;
        }
    }
};