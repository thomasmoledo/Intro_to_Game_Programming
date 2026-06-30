#ifndef PHASE2_H
#define PHASE2_H

#include "Phase1.h"

class Phase2 : public Scene
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

    Phase2();
    Phase2(Vector2 origin, const char *bgHexCode);
    ~Phase2();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;
    void renderUI() override;
    void shutdown() override;
};

#endif // PHASE2_H