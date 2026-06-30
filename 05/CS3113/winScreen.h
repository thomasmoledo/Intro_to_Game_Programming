#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include "MainMenu.h"

class WinScreen : public Scene
{
private:
    Texture2D background;
    Sound winSound;

public:
    WinScreen();
    WinScreen(Vector2 origin, const char *bgHexCode);
    ~WinScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;
    void renderUI() override { return; }
    void shutdown() override;
};

#endif // WIN_SCREEN_H