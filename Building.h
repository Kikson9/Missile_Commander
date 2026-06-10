#pragma once

#include "Defines.h"

class Building
{
public:
    Vector2 position;
    bool active;

    Building()
    {
        position = {0, 0};
        active = true;
    }

    void Reset()
    {
        active = true;
    }

    void Hit()
    {
        active = false;
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
            position.x - BUILDING_SIZE / 2,
            position.y - BUILDING_SIZE / 2,
            BUILDING_SIZE, BUILDING_SIZE, LIGHTGRAY);
    }
};