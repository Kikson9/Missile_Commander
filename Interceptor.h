#pragma once

#include "Defines.h"

class Interceptor
{
public:
    Vector2 origin;
    Vector2 position;
    Vector2 objective;
    Vector2 speed;
    bool active;

    Interceptor()
    {
        origin = {0, 0};
        position = {0, 0};
        objective = {0, 0};
        speed = {0, 0};
        active = false;
    }

    void Reset()
    {
        active = false;
    }

    bool IsActive()
    {
        return active;
    }

    void Draw(int frame)
    {
        if (!active)
            return;
        DrawLine(origin.x, origin.y, position.x, position.y, GREEN);
        if (frame % 16 < 8)
            DrawCircle(position.x, position.y, 3, BLUE);
    }

    void Update()
    {
        if (!active)
            return;
        position.x += speed.x;
        position.y += speed.y;
    }
};