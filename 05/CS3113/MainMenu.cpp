#include "MainMenu.h"

MainMenu::MainMenu()                                      : Scene { {0.0f}, nullptr   } {}
MainMenu::MainMenu(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
MainMenu::~MainMenu() { shutdown(); }

 void MainMenu::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    
    background = LoadTexture("assets/mainMenu.png");
    controls = LoadTexture("assets/controls.png");
    playButton = LoadTexture("assets/playButton.png");
    controlsButton = LoadTexture("assets/controlsButton.png");
    quitButton = LoadTexture("assets/quitButton.png");

    mGameState.clickSound = LoadSound("assets/clickSound.mp3");
    isControls = false;

    // Buttons
    mGameState.buttons = new Entity[3];
    for (int i = 0; i < 3; ++i) {
        mGameState.buttons[i].setTexture(buttonTextures[i]);
        mGameState.buttons[i].setEntityType(EMPTY);
        mGameState.buttons[i].setScale({400.0f, 150.0f});
        mGameState.buttons[i].setColliderDimensions({375.0f, 120.0f});
        mGameState.buttons[i].setPosition( ORIGIN );
    }

    // Button Positions
    mGameState.buttons[0].setPosition({ ORIGIN.x - 875.0f, ORIGIN.y - 150.0f }); // Play
    mGameState.buttons[1].setPosition({ ORIGIN.x - 875.0f, ORIGIN.y + 50.0f }); // Controls
    mGameState.buttons[2].setPosition({ ORIGIN.x - 875.0f, ORIGIN.y + 250.0f }); // Quit
}

void MainMenu::render() {}

void MainMenu::update(float deltaTime) {}

void MainMenu::renderBackground()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderObject(&background, &ORIGIN, &SCREEN_SIZE);
    for (int i = 0; i < 3; ++i) mGameState.buttons[i].render();
    if (isControls) renderObject(&controls, &ORIGIN, &SCREEN_SIZE);
}

void MainMenu::shutdown()
{
    UnloadTexture(background);
    UnloadTexture(controls);
    UnloadTexture(playButton);
    UnloadTexture(quitButton);
}

void MainMenu::showControls() { isControls = true; }
void MainMenu::hideControls() { isControls = false; }