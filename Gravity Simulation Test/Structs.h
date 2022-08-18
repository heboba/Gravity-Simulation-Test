#pragma once

enum Type {
    Good, Top, Left, Right, Bottom
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
};
class Entity : public Object{
public:
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
        pos = pos + MoveVec;
        if (pos.y < 0) {
            pos.y = 0;
            MoveVec.y = 0;
        }

        this->MoveVec.x = 0;
        if ((GetKeyState(65) & 0x8000) != 0) this->MoveVec.x -= 0.5;
        if ((GetKeyState(68) & 0x8000) != 0) this->MoveVec.x += 0.5;
        if ((GetKeyState(32) & 0x8000) != 0) {
            if (OnGround || pos.y == 0)
            {
                this->Jump();
                JumpCnt = 1;
                JumpPressed = true;
            }
            else if (JumpCnt == 1 && MoveVec.y <= 0 && !JumpPressed) {
                this->Jump();
                JumpCnt = 0;
            }
        }
        else
        {
            JumpPressed = false;
        }
    }
};