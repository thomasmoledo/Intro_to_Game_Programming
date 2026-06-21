#ifndef LevelC_H
#define LevelC_H

#include "LevelB.h"

class LevelC : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,197, 20,  0,  0,  0,197,  0,  0,  0,197,  0,  0,  0, 18,217,218,  0,
        0,241,  0,  0,  0,  0,241,  0,  0,  0,241,  0,  0,  0,  0,239,240,  0
    };
    

public:
    static constexpr float TILE_DIMENSION          = 80.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();

    bool takeDamage( int *currHealth );

    void initialise() override;
    void update(float deltaTime) override;
    void update(float deltaTime, int *currHealth) override;
    void render() override;
    void shutdown() override;
};

#endif // LevelC_H
