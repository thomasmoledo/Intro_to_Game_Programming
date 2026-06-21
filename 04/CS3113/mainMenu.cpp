#include "MainMenu.h"

MainMenu::MainMenu()                                      : Scene { {0.0f}, nullptr   } {}
MainMenu::MainMenu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
MainMenu::~MainMenu() { shutdown(); }

void MainMenu::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mainMenu = LoadTexture("assets/mainMenu.png");
    mGameState.clickSound = LoadSound("assets/clickSound.mp3");
}

void MainMenu::update(float deltaTime) {}

void MainMenu::update(float deltaTime, int *currHealth) {}

void MainMenu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderObject(&mainMenu, { ORIGIN.x, ORIGIN.y }, { SCREEN_WIDTH, SCREEN_HEIGHT }, 0.0f);
}

void MainMenu::shutdown()
{
    UnloadTexture(mainMenu);
    UnloadSound(mGameState.clickSound);
}
