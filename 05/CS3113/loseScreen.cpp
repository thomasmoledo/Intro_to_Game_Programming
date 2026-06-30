#include "loseScreen.h"

LoseScreen::LoseScreen()                                      : Scene { {0.0f}, nullptr   } {}
LoseScreen::LoseScreen(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
LoseScreen::~LoseScreen() { shutdown(); }

void LoseScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    
    background = LoadTexture("assets/loseScreen.png");
    mGameState.clickSound = LoadSound("assets/clickSound.mp3");
    loseSound = LoadSound("assets/loseSound.mp3");
    PlaySound(loseSound);
}

void LoseScreen::update(float deltaTime) {}

void LoseScreen::render() {}

void LoseScreen::renderBackground()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderObject(&background, &ORIGIN, &SCREEN_SIZE);
}

void LoseScreen::shutdown()
{
    UnloadTexture(background);
    UnloadSound(loseSound);
}