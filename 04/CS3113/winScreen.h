#ifndef WINSCREEN_H
#define WINSCREEN_H

#include "mainMenu.h"

class WinScreen : public Scene
{
private:
    Texture2D winScreen;

public:
    WinScreen();
    WinScreen(Vector2 origin, const char *bgHexCode);
    ~WinScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void update(float deltaTime, int *currHealth) override;
    void render() override;
    void shutdown() override;
};

#endif // WINSCREEN_H
