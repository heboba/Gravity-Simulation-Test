#pragma once
struct Pos {
    float x, y;
    Pos operator+(Pos pos2) {
        return  { x + pos2.x,y + pos2.y };
    }
    Pos operator-(Pos pos2) {
        return  { x - pos2.x,y - pos2.y };
    }
};
class Object {
public:
    Pos pos;
    Pos pos2;
    Pos MoveVec;
    void Move() {
        pos = pos + MoveVec;
        if (pos.y < 0) {
            pos.y = 0;
            MoveVec.y = 0;
        }
    }
};