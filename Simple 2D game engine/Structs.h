#pragma once

#include <iostream>

enum Type {
    Top = 1, Left = -2, Right = 2, Bottom = -1, Error = 404
};
enum ObjectType {
    tPlayer, tObject
};
enum EditActionType {
    None, ChangeX, ChangeY, ScaleMode, ChangeUfps
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
    ObjectType type;
    float color[3] = { 1,1,1 };
    std::string texture;
    Object(float x1, float y1, float x2, float y2) {
        pos = { x1,y1 };
        pos2 = { x2,y2 };
        type = tObject;
        texture = "yellow_shulker_box.png";
    }
    Object() {
        pos = { 0,0 };
        pos2 = { 0,0 };
        type = tObject;
        texture = "yellow_shulker_box.png";
    }
    float Get(char what) {
        switch (what)
        {
        case 'T':
            return pos.y + pos2.y;
        case 'D':
            return pos.y;
        case 'L':
            return pos.x;
        case 'R':
            return pos.x + pos2.x;
        }
    }
    Pos Get(std::string what) {
        if (what == "TL") return { Get('L'),Get('T') };
        if (what == "TR") return { Get('R'),Get('T') };
        if (what == "DL") return { Get('L'),Get('D') };
        if (what == "DR") return { Get('R'),Get('D') };
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
    Player() {
        type = tPlayer;
    }
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