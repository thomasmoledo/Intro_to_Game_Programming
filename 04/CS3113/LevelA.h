#ifndef LEVELA_H
#define LEVELA_H

#include "loseScreen.h"

constexpr int LEVEL_WIDTH  = 18,
              LEVEL_HEIGHT = 8;

class LevelA : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0, 35, 40, 42, 35,  0,  0,  0,  0, 36, 37,  0,
        0, 13, 15,  0,  0, 13, 14, 15,  0,  0, 13, 14, 15,  0,  0, 58, 59,  0
    };

public:
    static constexpr float TILE_DIMENSION          = 80.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    bool takeDamage( int *currHealth );

    void initialise() override;
    void update(float deltaTime) override;
    void update(float deltaTime, int *currHealth) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELA_H
