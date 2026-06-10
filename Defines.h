#pragma once

#include "raylib.h"
#include <raymath.h>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define MAX_MISSILES 100
#define MAX_INTERCEPTORS 30
#define MAX_EXPLOSIONS 100
#define LAUNCHERS_AMOUNT 3
#define BUILDINGS_AMOUNT 6

#define LAUNCHER_SIZE 80
#define BUILDING_SIZE 60
#define EXPLOSION_RADIUS 40

#define MISSILE_LAUNCH_FRAMES 80
#define INTERCEPTOR_SPEED 10
#define EXPLOSION_INCREASE_TIME 90
#define EXPLOSION_TOTAL_TIME 210

#define EXPLOSION_COLOR (Color){125, 125, 125, 125}

//----------------------------------------------------------------------------------
// Enum
//----------------------------------------------------------------------------------
enum class WaveState
{
    PLAYING,
    WAVE_CLEAR,
    WAVE_INCOMING
};