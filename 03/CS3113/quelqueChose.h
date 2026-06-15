#ifndef QC_H
#define QC_H

#include "Entity.h"
#include "perlin_noise.h"

enum Menu        { MAIN, GAME, END };
enum PauseStatus { PLAYING, PAUSED };
enum MusicStatus {     ON, OFF     };
enum GameStatus  {  WIN, LOSE, NA  };

struct GameState 
{
    Entity *lander  = nullptr,
           *buttons = nullptr;

    float *heightmap = nullptr;

    int currFuel = 0;

    Music bgm;
    Sound success;
    Sound fail;

    Texture2D mainMenu;
    Texture2D pauseMenu;
    Texture2D winScreen;
    Texture2D loseScreen;
};

void renderTerrain(int SCREEN_WIDTH, int SCREEN_HEIGHT, int seed,
                     float freq, float amplifier, float offset, int octaves,
                     float landZone1, float landZone2, float landZoneSize);

void generateHeightmap(GameState *gState, int SCREEN_WIDTH, int SCREEN_HEIGHT, int seed,
                       float freq, float amplifier, float offset, int octaves,
                       float landZone1, float landZone2, float landZoneSize);

void generateNewSeed(int *seed, float *landZone1, float *landZone2, int SCREEN_WIDTH);

void checkCollision(GameState *gState, float landZone1, float landZone2, float landZoneSize,
                    GameStatus *gGameStatus, Menu *gMenu, int SCREEN_WIDTH);

void resetGame(GameState *gState, Vector2 ORIGIN, int MAX_FUEL, GameStatus *gGameStatus);

void crash(Entity* lander, Menu *gMenu, GameStatus *gGameStatus);

void accelerateUp(GameState *gState, float THRUST_ACCELERATION);
void accelerateDown(GameState *gState, float THRUST_ACCELERATION);
void accelerateLeft(GameState *gState, float THRUST_ACCELERATION);
void accelerateRight(GameState *gState, float THRUST_ACCELERATION);

#endif // QC.H