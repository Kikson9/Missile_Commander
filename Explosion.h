#pragma once

#include "Defines.h"

class Explosion
{
public:
    Vector2 position;
    float radiusMultiplier;
    int frame;
    bool active;

    Explosion()
    {
        position = {0, 0};
        radiusMultiplier = 0.0f;
        frame = 0;
        active = false;
    }

    void Reset()
    {
        active = false;
        frame = 0;
        radiusMultiplier = 0.0f;
    }

    bool IsActive()
    {
        return active;
    }

    void Draw()
    {
        if (!active)
            return;
        DrawCircle(position.x, position.y,
                   EXPLOSION_RADIUS * radiusMultiplier, EXPLOSION_COLOR);
    }

    void Update()
    {
        if (!active)
            return;
        frame++;
        if (frame <= EXPLOSION_INCREASE_TIME)
            radiusMultiplier = frame / (float)EXPLOSION_INCREASE_TIME;
        else if (frame <= EXPLOSION_TOTAL_TIME)
            radiusMultiplier = 1 - (frame - (float)EXPLOSION_INCREASE_TIME) /
                                       (float)EXPLOSION_TOTAL_TIME;
        else
            Reset();
    }
};