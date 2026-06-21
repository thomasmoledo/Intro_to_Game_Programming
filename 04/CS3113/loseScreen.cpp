#include "loseScreen.h"

LoseScreen::LoseScreen()                                      : Scene { {0.0f}, nullptr   } {}
LoseScreen::LoseScreen(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
LoseScreen::~LoseScreen() { shutdown(); }

void LoseScreen::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    loseScreen = LoadTexture("assets/loseScreen.png");
    mGameState.clickSound = LoadSound("assets/clickSound.mp3");
    mGameState.loseSound = LoadSound("assets/loseSound.mp3");
    PlaySound(mGameState.loseSound);
}

void LoseScreen::update(float deltaTime) {}

void LoseScreen::update(float deltaTime, int *currHealth) {}

void LoseScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderObject(&loseScreen, { ORIGIN.x, ORIGIN.y }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 0.0f);
}

void LoseScreen::shutdown()
{
    UnloadTexture(loseScreen);
    UnloadSound(mGameState.clickSound);
    UnloadSound(mGameState.loseSound);
}
