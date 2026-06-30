#include "winScreen.h"

WinScreen::WinScreen()                                      : Scene { {0.0f}, nullptr   } {}
WinScreen::WinScreen(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
WinScreen::~WinScreen() { shutdown(); }

void WinScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    
    background = LoadTexture("assets/winScreen.png");
    mGameState.clickSound = LoadSound("assets/clickSound.mp3");
    winSound = LoadSound("assets/winSound.mp3");
    PlaySound(winSound);
}

void WinScreen::update(float deltaTime) {}

void WinScreen::render() {}

void WinScreen::renderBackground()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderObject(&background, &ORIGIN, &SCREEN_SIZE);
}

void WinScreen::shutdown()
{
    UnloadTexture(background);
    UnloadSound(winSound);
}