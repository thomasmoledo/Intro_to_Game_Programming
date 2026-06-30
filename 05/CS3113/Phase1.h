#ifndef PHASE1_H
#define PHASE1_H

#include "Scene.h"

class Phase1 : public Scene
{
private:
    Texture2D background,
              bossHealthBar,
              playerHealthBar;

    float bossHealthRatio,
          bossDamage,
          bossShootSpeed,
          bossHealth,
          
          playerHealthRatio,
          playerDamage,
          playerShootSpeed,
          playerHealth;

public:
    static constexpr float ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f,
                           RES_RATIO               = 2560.0f / 1600.0f;

    Phase1();
    Phase1(Vector2 origin, const char *bgHexCode);
    ~Phase1();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;
    void renderUI() override;
    void shutdown() override;
};

#endif // PHASE1_H