#ifndef LOSE_SCREEN_H
#define LOSE_SCREEN_H

#include "winScreen.h"

class LoseScreen : public Scene
{
private:
    Texture2D background;
    Sound loseSound;

public:
    LoseScreen();
    LoseScreen(Vector2 origin, const char *bgHexCode);
    ~LoseScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;
    void renderUI() override { return; }
    void shutdown() override;
};

#endif // LOSE_SCREEN_H