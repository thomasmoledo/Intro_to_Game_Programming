// main.cpp
#include "CS3113/Entity.h"
#include "CS3113/quelqueChose.h"
#include "CS3113/perlin_noise.h" // https://github.com/2G-Afroz/perlin-noise/tree/main

// Global Constants
constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 900,
              FPS           = 120;

constexpr char  BG_COLOUR[] = "#000000",

                // Buttons
                START      [] = "assets/startButton.png",
                QUIT       [] = "assets/quitButton.png",
                RESUME     [] = "assets/resumeButton.png",
                RESTART    [] = "assets/restartButton.png",
                QUIT2      [] = "assets/quitButton.png",
                RESTART2   [] = "assets/restartButton.png",
                QUIT3      [] = "assets/quitButton.png";

constexpr int NUMBER_OF_BUTTONS = 7,
              MAX_FUEL          = 8 * FPS;

const char *buttonTextures[NUMBER_OF_BUTTONS] = { START, QUIT, RESUME, RESTART, QUIT2, RESTART2, QUIT3 };

constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  RESOLUTION       = { SCREEN_WIDTH, SCREEN_HEIGHT },
                  ATLAS_DIMENSIONS = { 6, 8 },
                  MOUSE_SCALE      = { 1.0f, 1.0f },
                  BUTTON_SIZE      = { 320.0f, 180.0f };

constexpr float ACCELERATION_OF_GRAVITY = 981.0f / 95,
                FIXED_TIMESTEP          = 1.0f / 60.0f,
                END_GAME_THRESHOLD      = 800.0f,
                THRUST_ACCELERATION     = 1500.0f / 45.0f,
                DRAG                    = 0.25f,
                VELOCITY_LIMIT          = 20.0f; 

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

int octaves  = 2,
    seed     = GetRandomValue(0, 1000);

float freq         = 0.007,
      amplifier    = 350.0f,
      offset       = 650.0f,
      landZone1    = 0.0f,
      landZone2    = 0.0f,
      landZoneSize = 75.0f,
      bgAngle      = 0.0f;

Menu        gMenu        = MAIN;
PauseStatus gPauseStatus = PLAYING;
GameStatus  gGameStatus  = NA;
MusicStatus gMusicStatus = ON;

GameState gState;

Vector2 gMousePosition = GetMousePosition();

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Holy Overkill");
    InitAudioDevice();

    /*
    ----------- GAMESTATE -----------
    */
    gState.bgm = LoadMusicStream("assets/RyanGosling.mp3");
    SetMusicVolume(gState.bgm, 0.33f);
    PlayMusicStream(gState.bgm);

    // gState.success = LoadSound("assets/success.mp3");
    // gState.fail = LoadSound("assets/fail.mp3");

    gState.mainMenu = LoadTexture("assets/mainMenu.png");
    gState.pauseMenu = LoadTexture("assets/pauseMenu.png");
    gState.winScreen = LoadTexture("assets/winScreen.png");
    gState.loseScreen = LoadTexture("assets/loseScreen.png");

    // Buttons
    gState.buttons = new Entity[NUMBER_OF_BUTTONS];
    for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
        gState.buttons[i].setTexture(buttonTextures[i]);
        gState.buttons[i].setEntityType(NONE);
        gState.buttons[i].setScale({BUTTON_SIZE.x, BUTTON_SIZE.y});
        gState.buttons[i].setColliderDimensions({BUTTON_SIZE.x, BUTTON_SIZE.y});
        gState.buttons[i].setPosition( ORIGIN );
    }

    // Button Positions
    gState.buttons[0].setPosition({ ORIGIN.x - 300.0f, ORIGIN.y + 150.0f }); // Play
    gState.buttons[1].setPosition({ ORIGIN.x + 300.0f, ORIGIN.y + 150.0f }); // Quit
    gState.buttons[2].setPosition({ ORIGIN.x - 400.0f, ORIGIN.y + 150.0f }); // Resume
    gState.buttons[3].setPosition({ ORIGIN.x         , ORIGIN.y + 150.0f }); // Restart
    gState.buttons[4].setPosition({ ORIGIN.x + 400.0f, ORIGIN.y + 150.0f }); // Quit2
    gState.buttons[5].setPosition({ ORIGIN.x - 300.0f, ORIGIN.y + 150.0f }); // Restart2
    gState.buttons[6].setPosition({ ORIGIN.x + 300.0f, ORIGIN.y + 150.0f }); // Quit3
    

    /*
    ------------- LANDER -------------
    */
    float sizeRatio = 1117.0f / 1465.0f;

    gState.lander = new Entity(
        {ORIGIN.x, ORIGIN.y - 300.0f}, // position
        {75.0f * sizeRatio, 75.0f},    // scale
        "assets/kerbal.png",           // texture file address
        PLAYER                         // entity type
    );

    gState.lander->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    gState.currFuel = MAX_FUEL;

    /*
    ------------ TERRAIN ------------
    */
    generateNewSeed(&seed, &landZone1, &landZone2, SCREEN_WIDTH);
    generateHeightmap(&gState, SCREEN_WIDTH, SCREEN_HEIGHT, seed, freq, amplifier,
                      offset, octaves, landZone1, landZone2, landZoneSize);

    SetTargetFPS(FPS);
}

void processInput()
{
    gMousePosition = GetMousePosition();

    gState.lander->resetMovement();
    
    if (!gState.lander->isAccelerating())
        gState.lander->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    if (gMenu == MAIN)
    {
        if (isColliding(gState.buttons[0].getPosition(), gState.buttons[0].getScale(), &gMousePosition, &MOUSE_SCALE))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gMenu = GAME;
        
        if (isColliding(gState.buttons[1].getPosition(), gState.buttons[1].getScale(), &gMousePosition, &MOUSE_SCALE))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gAppStatus = TERMINATED;
    }

    if (gMenu == GAME)
    {
        if (IsKeyPressed(KEY_P)) gPauseStatus = (gPauseStatus == PAUSED) ? PLAYING : PAUSED;

        if (gPauseStatus == PLAYING)
        {
            if      (IsKeyDown(KEY_W) && gState.currFuel > 0) { accelerateUp(&gState, THRUST_ACCELERATION); }
            else if (IsKeyDown(KEY_S) && gState.currFuel > 0) { accelerateDown(&gState, THRUST_ACCELERATION); }
            if      (IsKeyDown(KEY_A) && gState.currFuel > 0) { accelerateLeft(&gState, THRUST_ACCELERATION); }
            else if (IsKeyDown(KEY_D) && gState.currFuel > 0) { accelerateRight(&gState, THRUST_ACCELERATION); }
        }

        if (gPauseStatus == PAUSED)
        {
            if (isColliding(gState.buttons[2].getPosition(), gState.buttons[2].getScale(), &gMousePosition, &MOUSE_SCALE))
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gPauseStatus = PLAYING;
            
            if (isColliding(gState.buttons[3].getPosition(), gState.buttons[3].getScale(), &gMousePosition, &MOUSE_SCALE))
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    resetGame(&gState, ORIGIN, MAX_FUEL, &gGameStatus);
                    gPauseStatus = PLAYING;
                }

            if (isColliding(gState.buttons[4].getPosition(), gState.buttons[4].getScale(), &gMousePosition, &MOUSE_SCALE))
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    resetGame(&gState, ORIGIN, MAX_FUEL, &gGameStatus);
                    generateNewSeed(&seed, &landZone1, &landZone2, SCREEN_WIDTH);
                    generateHeightmap(&gState, SCREEN_WIDTH, SCREEN_HEIGHT, seed, freq, amplifier,
                      offset, octaves, landZone1, landZone2, landZoneSize);
                    gMenu = MAIN;
                    gPauseStatus = PLAYING;
                }
        }
    }

    if (gMenu == END)
    {
        if (isColliding(gState.buttons[5].getPosition(), gState.buttons[5].getScale(), &gMousePosition, &MOUSE_SCALE))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                resetGame(&gState, ORIGIN, MAX_FUEL, &gGameStatus);
                generateNewSeed(&seed, &landZone1, &landZone2, SCREEN_WIDTH);
                generateHeightmap(&gState, SCREEN_WIDTH, SCREEN_HEIGHT, seed, freq, amplifier,
                    offset, octaves, landZone1, landZone2, landZoneSize);
                gMenu = MAIN;
                gPauseStatus = PLAYING;
            }
        
        if (isColliding(gState.buttons[6].getPosition(), gState.buttons[6].getScale(), &gMousePosition, &MOUSE_SCALE))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gAppStatus = TERMINATED;
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks    = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        if (gMenu == GAME && gPauseStatus == PLAYING)
        {
            gState.lander->update(FIXED_TIMESTEP, nullptr, nullptr, nullptr, 0, DRAG);
            checkCollision(&gState, landZone1, landZone2, landZoneSize, &gGameStatus, &gMenu, SCREEN_WIDTH, VELOCITY_LIMIT);
        }

        if (gMusicStatus == ON) UpdateMusicStream(gState.bgm);
        
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    if (gMenu == MAIN)
    {
        renderObject(&gState.mainMenu, &ORIGIN, &RESOLUTION, &bgAngle);
        gState.buttons[0].render();
        gState.buttons[1].render();
    }

    else if (gMenu == GAME)
    {
        if (gPauseStatus == PLAYING)
        {
            gState.lander->render();
            renderTerrain(SCREEN_WIDTH, SCREEN_HEIGHT, seed, freq, amplifier,
                        offset, octaves, landZone1, landZone2, landZoneSize);
            DrawText(TextFormat("%d", gState.currFuel), 75, 40, 60, WHITE);
        }

        else if (gPauseStatus == PAUSED)
        {
            renderObject(&gState.pauseMenu, &ORIGIN, &RESOLUTION, &bgAngle);
            gState.buttons[2].render();
            gState.buttons[3].render();
            gState.buttons[4].render();
        }
    }

    else if (gMenu == END)
    {
        if (gGameStatus == WIN)
        {
            renderObject(&gState.winScreen, &ORIGIN, &RESOLUTION, &bgAngle);
            gState.buttons[5].render();
            gState.buttons[6].render();
        }

        else if (gGameStatus == LOSE)
        {
            renderObject(&gState.loseScreen, &ORIGIN, &RESOLUTION, &bgAngle);
            gState.buttons[5].render();
            gState.buttons[6].render();
        }
    }

    EndDrawing();
}

void shutdown()
{
    delete gState.lander;
    delete [] gState.buttons;
    delete [] gState.heightmap;
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
