#include "quelquechose.h"

void renderTerrain(int SCREEN_WIDTH, int SCREEN_HEIGHT, int seed,
                     float freq, float amplifier, float offset, int octaves,
                     float landZone1, float landZone2, float landZoneSize)
{
    float a = seed;
    for(int i = 0; i< SCREEN_WIDTH; i += (i == landZone1 || i == landZone2) ? landZoneSize : 1)
    {
        if (i == landZone1 || i == landZone2)
        {
            DrawLine(i, perlinNoise(a, octaves) * amplifier + offset,
                i + landZoneSize, perlinNoise(a, octaves) * amplifier + offset,
                GREEN);
        }
        
        else
        {
            DrawLine(i, perlinNoise(a, octaves) * amplifier + offset,
                i + 1, perlinNoise(a + freq, octaves) * amplifier + offset,
                WHITE);
        
            a += freq;
        }            
    }
}

void generateHeightmap(GameState *gState, int SCREEN_WIDTH, int SCREEN_HEIGHT, int seed,
                       float freq, float amplifier, float offset, int octaves,
                       float landZone1, float landZone2, float landZoneSize)
{
    if (gState->heightmap) delete [] gState->heightmap;
    gState->heightmap = new float[SCREEN_WIDTH];

    float a = seed;
    for(int i = 0; i< SCREEN_WIDTH; i += (i == landZone1 || i == landZone2) ? landZoneSize : 1)
    {
        if (i == landZone1 || i == landZone2) 
        {
            float landZoneHeight = perlinNoise(a, octaves) * amplifier + offset;

            for (int j = 0; j < landZoneSize && (i + j) < SCREEN_WIDTH; ++j)
                gState->heightmap[i + j] = landZoneHeight;
        }

        else
        {
            gState->heightmap[i] = perlinNoise(a, octaves) * amplifier + offset;
            a += freq;
        }
    }
}

void generateNewSeed(int *seed, float *landZone1, float *landZone2, int SCREEN_WIDTH)
{
    *seed = GetRandomValue(0, 80000);
    *landZone1 = (float) GetRandomValue(200, SCREEN_WIDTH - 200);
    *landZone2 = (float) GetRandomValue(200, SCREEN_WIDTH - 200);
    while (fabs(*landZone2 - *landZone1) < 100.0f)
        *landZone2 = (float) GetRandomValue(200, SCREEN_WIDTH - 200);
}

void checkCollision(GameState *gState, float landZone1, float landZone2, float landZoneSize,
                    GameStatus *gGameStatus, Menu *gMenu, int SCREEN_WIDTH)
{
    if (!gState->heightmap)
        return;

    bool overLandingZone = false;
    float landerPositionX = gState->lander->getPosition().x,
          landerPositionY = gState->lander->getPosition().y + gState->lander->getScale().y / 2,
          landerWidth     = gState->lander->getScale().x;

    // Check if the lander is over a Landing Zone
    if (((landerPositionX - landerWidth / 2) > landZone1 &&
        (landerPositionX + landerWidth / 2) < landZone1 + landZoneSize) ||
        ((landerPositionX - landerWidth / 2) > landZone2 &&
        (landerPositionX + landerWidth / 2) < landZone2 + landZoneSize))
        {
            overLandingZone = true;
        }

    // Check if the lander is above the ground. If not, check win condition to decide result
    int leftEdge  = static_cast<int>(landerPositionX - landerWidth / 2);
    int rightEdge = static_cast<int>(landerPositionX + landerWidth / 2);

    if (leftEdge < 0) crash(gState->lander, gMenu, gGameStatus);
    if (rightEdge >= SCREEN_WIDTH) crash(gState->lander, gMenu, gGameStatus);

    for (int i = leftEdge; i <= rightEdge; ++i)
    {
        if (landerPositionY > gState->heightmap[i])
        {
            if (overLandingZone)
            {
                if (gState->lander->getVelocity().x < 200.0f ||
                    gState->lander->getVelocity().y < 200.0f)
                {
                    *gGameStatus = WIN;
                    *gMenu = END;
                }

                else crash(gState->lander, gMenu, gGameStatus);
            }

            else crash(gState->lander, gMenu, gGameStatus);
        }
    }
}

void resetGame(GameState *gState, Vector2 ORIGIN, int MAX_FUEL, GameStatus *gGameStatus)
{
    gState->lander->setPosition({ORIGIN.x, ORIGIN.y - 300.0f});
    gState->lander->activate();
    gState->currFuel = MAX_FUEL;
    *gGameStatus = NA;
}

void crash(Entity* lander, Menu *gMenu, GameStatus *gGameStatus)
{
    lander->deactivate();
    *gGameStatus = LOSE;
    *gMenu = END;
}

void accelerateUp(GameState *gState, float THRUST_ACCELERATION)
{
    Vector2 acceleration = gState->lander->getAcceleration();
    acceleration.y -= THRUST_ACCELERATION;
    gState->lander->setAcceleration(acceleration);
    --gState->currFuel;
}
void accelerateDown(GameState *gState, float THRUST_ACCELERATION)
{
    Vector2 acceleration = gState->lander->getAcceleration();
    acceleration.y += THRUST_ACCELERATION;
    gState->lander->setAcceleration(acceleration);
    --gState->currFuel;
}

void accelerateLeft(GameState *gState, float THRUST_ACCELERATION)
{
    Vector2 acceleration = gState->lander->getAcceleration();
    acceleration.x -= THRUST_ACCELERATION;
    gState->lander->setAcceleration(acceleration);
    --gState->currFuel;
}

void accelerateRight(GameState *gState, float THRUST_ACCELERATION)
{
    Vector2 acceleration = gState->lander->getAcceleration();
    acceleration.x += THRUST_ACCELERATION;
    gState->lander->setAcceleration(acceleration);
    --gState->currFuel;
}