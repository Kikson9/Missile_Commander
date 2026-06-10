#pragma once

#include "Defines.h"

class Particle
{
public:
    Vector2 position;
    Vector2 velocity;
    float alpha;
    float size;
    bool active;

    Particle()
    {
        position = {0, 0};
        velocity = {0, 0};
        alpha = 0.0f;
        size = 0.0f;
        active = false;
    }

    void Reset()
    {
        active = false;
        alpha = 0.0f;
    }

    void Update()
    {
        if (!active)
            return;

        position.x += velocity.x;
        position.y += velocity.y;

        alpha -= 8.0f;

        if (alpha <= 0)
            Reset();
    }

    void Draw()
    {
        if (!active)
            return;
        DrawCircleV(position, size,
                    (Color){255, 200, 50, (unsigned char)alpha});
    }
};