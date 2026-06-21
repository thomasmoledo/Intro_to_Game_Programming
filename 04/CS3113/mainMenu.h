#ifndef MAINMENU_H
#define MAINMENU_H

#include "Scene.h"

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 4;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

class MainMenu : public Scene
{
private:
    Texture2D mainMenu;

public:
    MainMenu();
    MainMenu(Vector2 origin, const char *bgHexCode);
    ~MainMenu();

    void initialise() override;
    void update(float deltaTime) override;
    void update(float deltaTime, int *currHealth) override;
    void render() override;
    void shutdown() override;
};

#endif // MAINMENU_H
