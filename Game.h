#pragma once

#include "Defines.h"
#include "Missile.h"
#include "Interceptor.h"
#include "Explosion.h"
#include "Launcher.h"
#include "Building.h"
#include "Particle.h"

class Game
{
public:
    // Screen
    int screenWidth;
    int screenHeight;

    // Game state
    int framesCounter;
    bool gameOver;
    bool pause;
    int score;
    int highScore;
    int wave;
    int screenFlash;
    int bonusDisplay;
    int bonusTimer;

    // Wave logic
    WaveState waveState;
    int missilesThisWave;
    int missilesLaunched;
    int missilesDestroyed;
    float missileSpeed;
    int waveTimer;

    // Game objects
    Missile missile[MAX_MISSILES];
    Interceptor interceptor[MAX_INTERCEPTORS];
    Explosion explosion[MAX_EXPLOSIONS];
    Launcher launcher[LAUNCHERS_AMOUNT];
    Building building[BUILDINGS_AMOUNT];
    int explosionIndex;

    // Particles
    static const int MAX_PARTICLES = 200;
    Particle particles[200];

    // Starfield
    Vector2 stars[150];
    int starBrightness[150];

    // Constructor
    Game();

    // Core methods
    void Update();
    void Draw();
    void Reset();

private:
    void UpdateOutgoingFire();
    void UpdateIncomingFire();
    void StartNextWave();
    void SaveHighScore();
    void LoadHighScore();
    void SpawnParticles(Vector2 position);
};