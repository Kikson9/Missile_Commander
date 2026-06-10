#pragma once

#include "Defines.h"

class Launcher
{
public:
    Vector2 position;
    bool active;

    Launcher()
    {
        position = {0, 0};
        active = true;
    }

    void Reset()
    {
        active = true;
    }

    bool IsActive()
    {
        return active;
    }

    void Draw()
    {
        if (!active)
            return;
        DrawRectangle(
            position.x - LAUNCHER_SIZE / 2,
            position.y - LAUNCHER_SIZE / 2,
            LAUNCHER_SIZE, LAUNCHER_SIZE, GRAY);
    }
};