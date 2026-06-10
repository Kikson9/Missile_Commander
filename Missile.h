#pragma once

#include "Defines.h"

class Missile
{
public:
    Vector2 origin;
    Vector2 position;
    Vector2 objective;
    Vector2 speed;
    bool active;
    bool isMirv;
    bool hasSplit;
    bool isSmart;
    bool leftBlocked;
    bool rightBlocked;

    Missile()
    {
        origin = {0, 0};
        position = {0, 0};
        objective = {0, 0};
        speed = {0, 0};
        active = false;
        isMirv = false;
        hasSplit = false;
        isSmart = false;
        leftBlocked = false;
        rightBlocked = false;
    }

    void Reset()
    {
        active = false;
        isMirv = false;
        hasSplit = false;
        isSmart = false;
        leftBlocked = false;
        rightBlocked = false;
    }

    bool IsActive()
    {
        return active;
    }

    void Draw(int frame)
    {
        if (!active)
            return;

        Color trailColor = isSmart ? MAGENTA : (isMirv ? SKYBLUE : RED);
        Color tipColor = isSmart ? WHITE : (isMirv ? WHITE : YELLOW);

        DrawLine(origin.x, origin.y, position.x, position.y, trailColor);
        if (frame % 16 < 8)
            DrawCircle(position.x, position.y, 3, tipColor);
    }

    void Update()
    {
        if (!active)
            return;
        position.x += speed.x;
        position.y += speed.y;
    }
};