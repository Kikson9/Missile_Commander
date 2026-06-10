#include "Game.h"

Game::Game()
{
    screenWidth = 800;
    screenHeight = 450;
    framesCounter = 0;
    gameOver = false;
    pause = false;
    score = 0;
    highScore = 0;
    wave = 1;
    screenFlash = 0;
    explosionIndex = 0;
    bonusDisplay = 0;
    bonusTimer = 0;

    waveState = WaveState::WAVE_INCOMING;
    missilesThisWave = 5;
    missilesLaunched = 0;
    missilesDestroyed = 0;
    missileSpeed = 1.0f;
    waveTimer = 180;

    int sparcing = screenWidth / (LAUNCHERS_AMOUNT + BUILDINGS_AMOUNT + 1);

    launcher[0].position = {(float)(1 * sparcing), (float)(screenHeight - LAUNCHER_SIZE / 2)};
    building[0].position = {(float)(2 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[1].position = {(float)(3 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[2].position = {(float)(4 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    launcher[1].position = {(float)(5 * sparcing), (float)(screenHeight - LAUNCHER_SIZE / 2)};
    building[3].position = {(float)(6 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[4].position = {(float)(7 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[5].position = {(float)(8 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    launcher[2].position = {(float)(9 * sparcing), (float)(screenHeight - LAUNCHER_SIZE / 2)};

    for (int i = 0; i < LAUNCHERS_AMOUNT; i++)
        launcher[i].active = true;
    for (int i = 0; i < BUILDINGS_AMOUNT; i++)
        building[i].active = true;

    for (int i = 0; i < 150; i++)
    {
        stars[i].x = GetRandomValue(0, screenWidth);
        stars[i].y = GetRandomValue(0, screenHeight - 60);
        starBrightness[i] = GetRandomValue(100, 255);
    }

    LoadHighScore();
}

void Game::Reset()
{
    framesCounter = 0;
    gameOver = false;
    pause = false;
    score = 0;
    wave = 1;
    screenFlash = 0;
    explosionIndex = 0;

    waveState = WaveState::WAVE_INCOMING;
    missilesThisWave = 5;
    missilesLaunched = 0;
    missilesDestroyed = 0;
    missileSpeed = 1.0f;
    waveTimer = 180;

    for (int i = 0; i < MAX_MISSILES; i++)
        missile[i].Reset();
    for (int i = 0; i < MAX_INTERCEPTORS; i++)
        interceptor[i].Reset();
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
        explosion[i].Reset();
    for (int i = 0; i < LAUNCHERS_AMOUNT; i++)
        launcher[i].Reset();
    for (int i = 0; i < BUILDINGS_AMOUNT; i++)
        building[i].Reset();
    for (int i = 0; i < MAX_PARTICLES; i++)
        particles[i].Reset();

    int sparcing = screenWidth / (LAUNCHERS_AMOUNT + BUILDINGS_AMOUNT + 1);

    launcher[0].position = {(float)(1 * sparcing), (float)(screenHeight - LAUNCHER_SIZE / 2)};
    building[0].position = {(float)(2 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[1].position = {(float)(3 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[2].position = {(float)(4 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    launcher[1].position = {(float)(5 * sparcing), (float)(screenHeight - LAUNCHER_SIZE / 2)};
    building[3].position = {(float)(6 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[4].position = {(float)(7 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    building[5].position = {(float)(8 * sparcing), (float)(screenHeight - BUILDING_SIZE / 2)};
    launcher[2].position = {(float)(9 * sparcing), (float)(screenHeight - LAUNCHER_SIZE / 2)};
}

void Game::StartNextWave()
{
    bonusDisplay = 0;
    for (int i = 0; i < BUILDINGS_AMOUNT; i++)
    {
        if (building[i].active)
        {
            score += 10;
            bonusDisplay += 10;
        }
    }
    bonusTimer = 180;

    wave++;
    missilesThisWave += 4;
    missileSpeed += 0.3f;
    missilesLaunched = 0;
    missilesDestroyed = 0;
    waveState = WaveState::WAVE_INCOMING;
    waveTimer = 180;
}

void Game::Update()
{
    if (!gameOver)
    {
        if (IsKeyPressed('P'))
            pause = !pause;
        if (IsKeyPressed('R'))
            Reset();

        // CHEAT KEYS — remove before presentation
        if (IsKeyPressed('W'))
        {
            wave++;
            missilesThisWave += 4;
            missileSpeed += 0.3f;
            missilesLaunched = missilesThisWave;
            missilesDestroyed = missilesThisWave;
            waveState = WaveState::WAVE_CLEAR;
            waveTimer = 1;
        }
        if (IsKeyPressed('G'))
        {
            wave = 5;
            missilesThisWave = 5 + (4 * 4);
            missileSpeed = 1.0f + (0.3f * 4);
            missilesLaunched = missilesThisWave;
            missilesDestroyed = missilesThisWave;
            waveState = WaveState::WAVE_CLEAR;
            waveTimer = 1;
        }

        if (!pause)
        {
            framesCounter++;

            if (waveState == WaveState::WAVE_INCOMING)
            {
                waveTimer--;
                if (bonusTimer > 0)
                    bonusTimer--;
                if (waveTimer <= 0)
                    waveState = WaveState::PLAYING;
                UpdateOutgoingFire();
            }
            else if (waveState == WaveState::WAVE_CLEAR)
            {
                waveTimer--;
                if (waveTimer <= 0)
                    StartNextWave();
            }
            else if (waveState == WaveState::PLAYING)
            {
                float distance;

                // Interceptors update
                for (int i = 0; i < MAX_INTERCEPTORS; i++)
                {
                    if (interceptor[i].active)
                    {
                        interceptor[i].Update();

                        distance = sqrt(
                            pow(interceptor[i].position.x - interceptor[i].objective.x, 2) +
                            pow(interceptor[i].position.y - interceptor[i].objective.y, 2));

                        if (distance < INTERCEPTOR_SPEED)
                        {
                            interceptor[i].Reset();

                            explosion[explosionIndex].position = interceptor[i].position;
                            explosion[explosionIndex].active = true;
                            explosion[explosionIndex].frame = 0;
                            explosionIndex++;
                            if (explosionIndex == MAX_EXPLOSIONS)
                                explosionIndex = 0;

                            break;
                        }
                    }
                }

                // Missiles update
                for (int i = 0; i < MAX_MISSILES; i++)
                {
                    if (missile[i].active)
                    {
                        missile[i].Update();

                        // Smart bomb steering
                        if (missile[i].isSmart)
                        {
                            float threatRange = 80.0f;
                            missile[i].leftBlocked = false;
                            missile[i].rightBlocked = false;

                            for (int e = 0; e < MAX_EXPLOSIONS; e++)
                            {
                                if (explosion[e].active)
                                {
                                    Vector2 away = Vector2Subtract(
                                        missile[i].position,
                                        explosion[e].position);

                                    float dist = sqrtf(away.x * away.x + away.y * away.y);

                                    if (dist < threatRange)
                                    {
                                        if (explosion[e].position.x < missile[i].position.x)
                                            missile[i].leftBlocked = true;
                                        else
                                            missile[i].rightBlocked = true;

                                        Vector2 nudge = Vector2Normalize(away);
                                        missile[i].speed.x += nudge.x * 0.5f;
                                        if (nudge.y > 0)
                                            missile[i].speed.y += nudge.y * 0.3f;
                                    }
                                }
                            }

                            // Cornered — split into 3
                            if (missile[i].leftBlocked && missile[i].rightBlocked && missile[i].active)
                            {
                                missile[i].active = false;
                                missilesDestroyed++;

                                float fanAngles[3] = {-0.6f, 0.0f, 0.6f};
                                for (int f = 0; f < 3; f++)
                                {
                                    for (int k = 0; k < MAX_MISSILES; k++)
                                    {
                                        if (!missile[k].active)
                                        {
                                            missile[k].active = true;
                                            missile[k].isMirv = false;
                                            missile[k].hasSplit = false;
                                            missile[k].isSmart = false;
                                            missile[k].leftBlocked = false;
                                            missile[k].rightBlocked = false;
                                            missile[k].origin = missile[i].position;
                                            missile[k].position = missile[i].position;

                                            float speedMag = missileSpeed * 1.8f;
                                            missile[k].speed = {
                                                sinf(fanAngles[f]) * speedMag,
                                                cosf(fanAngles[f]) * speedMag};
                                            missile[k].objective = {
                                                (float)GetRandomValue(20, screenWidth - 20),
                                                (float)(screenHeight + 10)};
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        // MIRV split check
                        if (missile[i].active && missile[i].isMirv &&
                            !missile[i].hasSplit &&
                            missile[i].position.y >= screenHeight / 2)
                        {
                            missile[i].hasSplit = true;

                            int targets[3] = {0, 2, 4};
                            for (int c = 0; c < 3; c++)
                            {
                                for (int k = 0; k < MAX_MISSILES; k++)
                                {
                                    if (!missile[k].active)
                                    {
                                        missile[k].active = true;
                                        missile[k].isMirv = false;
                                        missile[k].hasSplit = false;
                                        missile[k].origin = missile[i].position;
                                        missile[k].position = missile[i].position;
                                        missile[k].objective = building[targets[c]].active ? building[targets[c]].position : (Vector2){(float)GetRandomValue(20, screenWidth - 20), (float)(screenHeight + 10)};

                                        float dx = missile[k].objective.x - missile[k].origin.x;
                                        float dy = missile[k].objective.y - missile[k].origin.y;
                                        float dist = sqrt(dx * dx + dy * dy);
                                        missile[k].speed = {
                                            dx / dist * missileSpeed * 1.5f,
                                            dy / dist * missileSpeed * 1.5f};
                                        break;
                                    }
                                }
                            }

                            missile[i].active = false;
                            missilesDestroyed++;
                        }

                        if (missile[i].position.y > screenHeight)
                        {
                            missile[i].Reset();
                            missilesDestroyed++;
                        }
                        else
                        {
                            // Check collision with launchers
                            for (int j = 0; j < LAUNCHERS_AMOUNT; j++)
                            {
                                if (launcher[j].active)
                                {
                                    if (CheckCollisionPointRec(missile[i].position,
                                                               (Rectangle){
                                                                   launcher[j].position.x - LAUNCHER_SIZE / 2,
                                                                   launcher[j].position.y - LAUNCHER_SIZE / 2,
                                                                   LAUNCHER_SIZE, LAUNCHER_SIZE}))
                                    {
                                        SpawnParticles(missile[i].position);
                                        missile[i].Reset();
                                        launcher[j].active = false;
                                        missilesDestroyed++;

                                        explosion[explosionIndex].position = missile[i].position;
                                        explosion[explosionIndex].active = true;
                                        explosion[explosionIndex].frame = 0;
                                        explosionIndex++;
                                        if (explosionIndex == MAX_EXPLOSIONS)
                                            explosionIndex = 0;

                                        break;
                                    }
                                }
                            }

                            // Check collision with buildings
                            for (int j = 0; j < BUILDINGS_AMOUNT; j++)
                            {
                                if (building[j].active)
                                {
                                    if (CheckCollisionPointRec(missile[i].position,
                                                               (Rectangle){
                                                                   building[j].position.x - BUILDING_SIZE / 2,
                                                                   building[j].position.y - BUILDING_SIZE / 2,
                                                                   BUILDING_SIZE, BUILDING_SIZE}))
                                    {
                                        SpawnParticles(missile[i].position);
                                        missile[i].Reset();
                                        building[j].Hit();
                                        screenFlash = 8;
                                        missilesDestroyed++;

                                        explosion[explosionIndex].position = missile[i].position;
                                        explosion[explosionIndex].active = true;
                                        explosion[explosionIndex].frame = 0;
                                        explosionIndex++;
                                        if (explosionIndex == MAX_EXPLOSIONS)
                                            explosionIndex = 0;

                                        break;
                                    }
                                }
                            }

                            // Check collision with explosions
                            for (int j = 0; j < MAX_EXPLOSIONS; j++)
                            {
                                if (explosion[j].active)
                                {
                                    if (CheckCollisionPointCircle(missile[i].position,
                                                                  explosion[j].position,
                                                                  EXPLOSION_RADIUS * explosion[j].radiusMultiplier))
                                    {
                                        SpawnParticles(missile[i].position);
                                        missile[i].Reset();
                                        score += 100;
                                        missilesDestroyed++;

                                        explosion[explosionIndex].position = missile[i].position;
                                        explosion[explosionIndex].active = true;
                                        explosion[explosionIndex].frame = 0;
                                        explosionIndex++;
                                        if (explosionIndex == MAX_EXPLOSIONS)
                                            explosionIndex = 0;

                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                // Explosions update
                for (int i = 0; i < MAX_EXPLOSIONS; i++)
                    explosion[i].Update();

                // Particles update
                for (int i = 0; i < MAX_PARTICLES; i++)
                    particles[i].Update();

                // Screen flash update
                if (screenFlash > 0)
                    screenFlash--;

                // Fire logic
                UpdateOutgoingFire();
                if (missilesLaunched < missilesThisWave)
                    UpdateIncomingFire();

                // Wave end check
                if (missilesLaunched >= missilesThisWave &&
                    missilesDestroyed >= missilesThisWave)
                {
                    waveState = WaveState::WAVE_CLEAR;
                    waveTimer = 180;
                }

                // Game over check
                int checker = 0;
                for (int i = 0; i < LAUNCHERS_AMOUNT; i++)
                {
                    if (!launcher[i].active)
                        checker++;
                    if (checker == LAUNCHERS_AMOUNT)
                    {
                        if (score > highScore)
                            highScore = score;
                        SaveHighScore();
                        gameOver = true;
                    }
                }

                checker = 0;
                for (int i = 0; i < BUILDINGS_AMOUNT; i++)
                {
                    if (!building[i].active)
                        checker++;
                    if (checker == BUILDINGS_AMOUNT)
                    {
                        if (score > highScore)
                            highScore = score;
                        SaveHighScore();
                        gameOver = true;
                    }
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
            Reset();
    }
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    // Starfield
    for (int i = 0; i < 150; i++)
        DrawPixel(stars[i].x, stars[i].y,
                  (Color){255, 255, 255, (unsigned char)starBrightness[i]});

    if (!gameOver)
    {
        for (int i = 0; i < MAX_MISSILES; i++)
            missile[i].Draw(framesCounter);
        for (int i = 0; i < MAX_INTERCEPTORS; i++)
            interceptor[i].Draw(framesCounter);
        for (int i = 0; i < MAX_EXPLOSIONS; i++)
            explosion[i].Draw();
        for (int i = 0; i < MAX_PARTICLES; i++)
            particles[i].Draw();
        for (int i = 0; i < LAUNCHERS_AMOUNT; i++)
            launcher[i].Draw();
        for (int i = 0; i < BUILDINGS_AMOUNT; i++)
            building[i].Draw();

        // HUD
        DrawText(TextFormat("SCORE %06i", score), 20, 15, 30, GREEN);
        DrawText(TextFormat("BEST  %06i", highScore), 20, 50, 18, DARKGREEN);

        if (bonusTimer > 0)
        {
            unsigned char alpha = (unsigned char)(255 * bonusTimer / 180.0f);
            DrawText(TextFormat("(+%i)", bonusDisplay), 20, 72, 18,
                     (Color){0, 255, 0, alpha});
        }

        DrawText(TextFormat("WAVE %02i", wave),
                 screenWidth / 2 - 50, 15, 30, YELLOW);

        for (int i = 0; i < BUILDINGS_AMOUNT; i++)
        {
            Color iconColor = building[i].active ? BLUE : DARKGRAY;
            DrawRectangle(screenWidth - 30 - (i * 22), 15, 18, 18, iconColor);
        }

        if (waveState == WaveState::WAVE_INCOMING)
        {
            DrawText(TextFormat("WAVE %i INCOMING", wave),
                     screenWidth / 2 - MeasureText(
                                           TextFormat("WAVE %i INCOMING", wave), 40) /
                                           2,
                     screenHeight / 2 - 20, 40, RED);

            DrawText(TextFormat("STARTING IN %i...", waveTimer / 60 + 1),
                     screenWidth / 2 - MeasureText(
                                           TextFormat("STARTING IN %i...", waveTimer / 60 + 1), 20) /
                                           2,
                     screenHeight / 2 + 30, 20, WHITE);
        }
        else if (waveState == WaveState::WAVE_CLEAR)
        {
            float pulse = (sinf(framesCounter * 0.15f) + 1.0f) / 2.0f;
            unsigned char alpha = (unsigned char)(155 + pulse * 100);
            DrawText("WAVE CLEAR!",
                     screenWidth / 2 - MeasureText("WAVE CLEAR", 50) / 2,
                     screenHeight / 2 - 25, 50,
                     (Color){0, 225, 0, alpha});
        }

        if (pause)
            DrawText("GAME PAUSED",
                     screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2,
                     screenHeight / 2 - 40, 40, GRAY);
    }
    else
    {
        DrawText("PRESS [ENTER] TO PLAY AGAIN",
                 screenWidth / 2 - MeasureText(
                                       "PRESS [ENTER] TO PLAY AGAIN", 20) /
                                       2,
                 screenHeight / 2 - 50, 20, GRAY);
    }

    // Vignette flash
    if (screenFlash > 0)
    {
        unsigned char alpha = (unsigned char)(screenFlash * 20);
        int thickness = 60;
        DrawRectangle(0, 0, screenWidth, thickness,
                      (Color){255, 0, 0, alpha});
        DrawRectangle(0, screenHeight - thickness, screenWidth, thickness,
                      (Color){255, 0, 0, alpha});
        DrawRectangle(0, 0, thickness, screenHeight,
                      (Color){255, 0, 0, alpha});
        DrawRectangle(screenWidth - thickness, 0, thickness, screenHeight,
                      (Color){255, 0, 0, alpha});
    }

    EndDrawing();
}

void Game::UpdateOutgoingFire()
{
    static int interceptorNumber = 0;
    int launcherShooting = 0;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        launcherShooting = 1;
    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        launcherShooting = 2;
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        launcherShooting = 3;

    if (launcherShooting > 0 && launcher[launcherShooting - 1].active)
    {
        float module, sideX, sideY;

        interceptor[interceptorNumber].active = true;
        interceptor[interceptorNumber].origin = launcher[launcherShooting - 1].position;
        interceptor[interceptorNumber].position = interceptor[interceptorNumber].origin;
        interceptor[interceptorNumber].objective = GetMousePosition();

        module = sqrt(
            pow(interceptor[interceptorNumber].objective.x -
                    interceptor[interceptorNumber].origin.x,
                2) +
            pow(interceptor[interceptorNumber].objective.y -
                    interceptor[interceptorNumber].origin.y,
                2));

        sideX = (interceptor[interceptorNumber].objective.x -
                 interceptor[interceptorNumber].origin.x) *
                INTERCEPTOR_SPEED / module;
        sideY = (interceptor[interceptorNumber].objective.y -
                 interceptor[interceptorNumber].origin.y) *
                INTERCEPTOR_SPEED / module;

        interceptor[interceptorNumber].speed = {sideX, sideY};

        interceptorNumber++;
        if (interceptorNumber == MAX_INTERCEPTORS)
            interceptorNumber = 0;
    }
}

void Game::UpdateIncomingFire()
{
    static int missileIndex = 0;

    if (framesCounter % MISSILE_LAUNCH_FRAMES == 0)
    {
        float module, sideX, sideY;

        missile[missileIndex].active = true;
        missile[missileIndex].isMirv = (wave >= 3) && (GetRandomValue(0, 3) == 0);
        missile[missileIndex].hasSplit = false;
        missile[missileIndex].isSmart = (wave >= 5) && (GetRandomValue(0, 6) == 0);
        missile[missileIndex].origin = {
            (float)GetRandomValue(20, screenWidth - 20), -10.0f};
        missile[missileIndex].position = missile[missileIndex].origin;
        missile[missileIndex].objective = {
            (float)GetRandomValue(20, screenWidth - 20),
            (float)(screenHeight + 10)};

        module = sqrt(
            pow(missile[missileIndex].objective.x - missile[missileIndex].origin.x, 2) +
            pow(missile[missileIndex].objective.y - missile[missileIndex].origin.y, 2));

        sideX = (missile[missileIndex].objective.x -
                 missile[missileIndex].origin.x) *
                missileSpeed / module;
        sideY = (missile[missileIndex].objective.y -
                 missile[missileIndex].origin.y) *
                missileSpeed / module;

        missile[missileIndex].speed = {sideX, sideY};

        missileIndex++;
        if (missileIndex == MAX_MISSILES)
            missileIndex = 0;

        missilesLaunched++;
    }
}

void Game::SaveHighScore()
{
    ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << highScore;
        file.close();
    }
}

void Game::LoadHighScore()
{
    ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
}

void Game::SpawnParticles(Vector2 pos)
{
    int spawned = 0;

    for (int i = 0; i < MAX_PARTICLES && spawned < 8; i++)
    {
        if (!particles[i].active)
        {
            float angle = GetRandomValue(0, 360) * DEG2RAD;
            float speed = GetRandomValue(1, 4);

            particles[i].active = true;
            particles[i].position = pos;
            particles[i].velocity = {cosf(angle) * speed,
                                     sinf(angle) * speed};
            particles[i].alpha = 255.0f;
            particles[i].size = GetRandomValue(2, 4);

            spawned++;
        }
    }
}