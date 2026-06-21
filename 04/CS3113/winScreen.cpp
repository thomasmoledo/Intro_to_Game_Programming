#include "winScreen.h"

WinScreen::WinScreen()                                      : Scene { {0.0f}, nullptr   } {}
WinScreen::WinScreen(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
WinScreen::~WinScreen() { shutdown(); }

void WinScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    winScreen = LoadTexture("assets/winScreen.png");
    mGameState.clickSound = LoadSound("assets/clickSound.mp3");
    mGameState.winSound = LoadSound("assets/winSound.mp3");
    PlaySound(mGameState.winSound);
}

void WinScreen::update(float deltaTime) {}

void WinScreen::update(float deltaTime, int *currHealth) {}

void WinScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderObject(&winScreen, { ORIGIN.x, ORIGIN.y }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 0.0f);
}

void WinScreen::shutdown()
{
    UnloadTexture(winScreen);
    UnloadSound(mGameState.clickSound);
    UnloadSound(mGameState.winSound);
}
