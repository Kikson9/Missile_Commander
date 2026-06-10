#include "Game.h"

int main(void)
{
    InitWindow(800, 450, "Missile Commander CPP");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose())
    {
        game.Update();
        game.Draw();
    }

    CloseWindow();
    return 0;
}