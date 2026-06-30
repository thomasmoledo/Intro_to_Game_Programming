#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "ShaderProgram.h"

class MainMenu : public Scene
{
private:
    Texture2D background,
              controls,
              playButton,
              controlsButton,
              quitButton;

    const char *buttonTextures[3] = { "assets/playButton.png", "assets/controlsButton.png", "assets/quitButton.png" };
    bool isControls;

public:
    MainMenu();
    MainMenu(Vector2 origin, const char *bgHexCode);
    ~MainMenu();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;
    void renderUI() override { return; }
    void shutdown() override;
    void showControls() override;
    void hideControls() override;
};

#endif // MAIN_MENU_H