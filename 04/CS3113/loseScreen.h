#ifndef LOSESCREEN_H
#define LOSESCREEN_H

#include "winScreen.h"

class LoseScreen : public Scene
{
private:
    Texture2D loseScreen;

public:
    LoseScreen();
    LoseScreen(Vector2 origin, const char *bgHexCode);
    ~LoseScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void update(float deltaTime, int *currHealth) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSESCREEN_H
