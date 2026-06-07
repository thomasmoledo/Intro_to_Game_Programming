/**
* Author: Thomas Moledo
* Assignment: Pong Clone
* Date due: 06/06/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

enum Menu        { MAINMENU, GAME, WIN, LOSE };
enum Players     { ONE, TWO };
enum BallCount   { UN, DEUX, TROIS };
enum MusicStatus { ON, OFF };

// Global Constants
constexpr int SCREEN_WIDTH    = 1920,
              SCREEN_HEIGHT   = 1080,
              FPS             = 120,
              NUMBER_OF_BALLS = 3,
              NUMBER_OF_WALLS = 4;

constexpr float BALL_SIZE = 125.0f,
                WALL_WIDTH = 50.0f,
                WALL_PADDING = 75.0f;

constexpr char    BG_COLOUR[] = "#ffffff",
                  PURPLE_GB[] = "assets/purpleGB.png",
                  RED_GB   [] = "assets/redGB.png",
                  GREEN_GB [] = "assets/greenGB.png";

const char *ballTextures[3] = { PURPLE_GB, RED_GB, GREEN_GB };

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

Vector2 gBackgroundPos = ORIGIN,
        gBackgroundScale = { 1920, 1080 };
float   gBackgroundAngle = 0.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
Players gPlayerCount = ONE;
Menu gMenu = GAME;
Texture2D MenuBackground,
          GameBackground,
          WinScreen,
          LoseScreen;

MusicStatus gMusicStatus = ON;
Music HeavenlyAudio;

int currPlayerOneScore = 0,
    currPlayerTwoScore = 0;

int currBallCount = 1;

float gPreviousTicks   = 0.0f;

Entity *PlayerOne = nullptr,
       *PlayerTwo = nullptr,
       *Balls     = nullptr,
       *Walls     = nullptr;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gummy Bear Pong");

    // Music & Sound Effects
    InitAudioDevice();
    HeavenlyAudio = LoadMusicStream("assets/gummyBearSong.mp3");
    SetMusicVolume(HeavenlyAudio, 0.33f);
    PlayMusicStream(HeavenlyAudio);

    // Backgrounds
    // MenuBackground = LoadTexture("assets/menuBackground.png");
    GameBackground = LoadTexture("assets/gameBackground.png");
    WinScreen      = LoadTexture("assets/winScreen.png");
    LoseScreen     = LoadTexture("assets/loseScreen.png");

    // Players
    float sizeRatio  = 1.0f / 3.0f;

    PlayerOne = new Entity(
        {ORIGIN.x - (SCREEN_WIDTH / 2 - WALL_WIDTH - WALL_PADDING), ORIGIN.y}, // position
        {250.0f * sizeRatio, 250.0f},  // scale
        "assets/playerModel.png",        // texture file address
        PLAYER                         // entity type
    );

    PlayerTwo = new Entity(
        {ORIGIN.x + (SCREEN_WIDTH / 2 - WALL_WIDTH - WALL_PADDING), ORIGIN.y}, // position
        {250.0f * sizeRatio, 250.0f},  // scale
        "assets/playerModel.png",        // texture file address
        PLAYER                         // entity type
    );

    // Balls
    Balls = new Entity[NUMBER_OF_BALLS];
    for ( int i = 0; i < NUMBER_OF_BALLS; ++i )
    {
        Balls[i].setTexture(ballTextures[i]);
        Balls[i].setEntityType(BALL);
        Balls[i].setScale({BALL_SIZE, BALL_SIZE});
        Balls[i].setColliderDimensions({BALL_SIZE / 3, BALL_SIZE / 3});
        Balls[i].setPosition({ ORIGIN.x, ORIGIN.y + (i > 1 ? 1 : i) * static_cast<float>(pow(-1, i)) * 200 });
        Balls[i].setMovement({
            350.0f * static_cast<float>(pow(-1, i)),
            350.0f * static_cast<float>(pow(-1, i + 1))
        });
        Balls[i].setSpeedX(1.0f);
        Balls[i].setSpeedY(1.0f);
    }

    // Walls
    Walls = new Entity[NUMBER_OF_WALLS];
    for ( int i = 0; i < NUMBER_OF_WALLS; ++i )     {
        Walls[i].setTexture("assets/placeholder.png");
        Walls[i].setEntityType(WALL);

        if (i < 2) {
            Walls[i].setScale({WALL_WIDTH, 1080.0f});
            Walls[i].setColliderDimensions({WALL_WIDTH, 1080.0f});
        } else {
            Walls[i].setScale({1920.0f, WALL_WIDTH});
            Walls[i].setColliderDimensions({1920.0f, WALL_WIDTH - 20});
        }
    }

    // Wall Positions
    Walls[0].setPosition({ WALL_WIDTH / 2, ORIGIN.y }); // Left Wall
    Walls[1].setPosition({ SCREEN_WIDTH - WALL_WIDTH / 2, ORIGIN.y }); // Right Wall
    Walls[2].setPosition({ ORIGIN.x, WALL_WIDTH / 2 }); // Top Wall
    Walls[3].setPosition({ ORIGIN.x, SCREEN_HEIGHT - WALL_WIDTH / 2 + 10.0f }); // Bottom Wall

    SetTargetFPS(FPS);
}

void processInput() {
    PlayerOne->resetMovement();
    PlayerTwo->resetMovement();

    // Player Input
    if (gMenu == GAME) {
        if (IsKeyDown(KEY_W)) PlayerOne->moveUp();
        if (IsKeyDown(KEY_S)) PlayerOne->moveDown();
        
        if ( gPlayerCount == TWO ) {
            if (IsKeyDown(KEY_UP)) PlayerTwo->moveUp();
            if (IsKeyDown(KEY_DOWN)) PlayerTwo->moveDown();
        }
    }

    // Settings
    if (IsKeyPressed(KEY_M)) gMusicStatus  = (gMusicStatus == ON ? OFF : ON);

    if (gMenu == GAME) {
        if (IsKeyPressed(KEY_T))     gPlayerCount  = (gPlayerCount == ONE ? TWO : ONE);
        if (IsKeyPressed(KEY_ONE))   currBallCount = 1;
        if (IsKeyPressed(KEY_TWO))   currBallCount = 2;
        if (IsKeyPressed(KEY_THREE)) currBallCount = 3;
    }

    // Temporary
    if (IsKeyPressed(KEY_U)) gMenu = MAINMENU;
    if (IsKeyPressed(KEY_I)) gMenu = GAME;
    if (IsKeyPressed(KEY_O)) gMenu = WIN;
    if (IsKeyPressed(KEY_P)) gMenu = LOSE;

    // Exit Game
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    if ( gMenu == GAME ) {
        // Player(s)
        if ( gPlayerCount == ONE ) {
            int closestBall  = 0;
            int currBallDist = 0;
            float closestLen = 1000000.0f;
            for (int i = 0; i < currBallCount; ++i) {
                float xDistance = fabs(PlayerTwo->getPosition().x - Balls[i].getPosition().x);
                float yDistance = fabs(PlayerTwo->getPosition().y - Balls[i].getPosition().y);
                currBallDist = GetLength({ xDistance, yDistance });
                if ( currBallDist < closestLen) {
                    closestLen = currBallDist;
                    closestBall = i;
                }
            }

            if (PlayerTwo->getPosition().y > Balls[closestBall].getPosition().y &&
                fabs(PlayerTwo->getPosition().y - Balls[closestBall].getPosition().y) > 20.0f)
                PlayerTwo->moveUp();
            else if (PlayerTwo->getPosition().y < Balls[closestBall].getPosition().y &&
                fabs(PlayerTwo->getPosition().y - Balls[closestBall].getPosition().y) > 20.0f)
                PlayerTwo->moveDown();
        }

        PlayerOne->update(deltaTime, Balls, currBallCount, Walls, NUMBER_OF_WALLS);
        PlayerTwo->update(deltaTime, Balls, currBallCount, Walls, NUMBER_OF_WALLS);

        // Ball(s)
        for (int i = 0; i < currBallCount; i++) {
            Balls[i].setAngle( Balls[i].getAngle() + 20.0f * deltaTime);
            Balls[i].update(deltaTime, Walls, NUMBER_OF_WALLS, nullptr, 0);
            
            if (Balls[i].isScoreOne()) {
                currPlayerTwoScore++;
                Balls[i].setPosition(ORIGIN);
                Balls[i].setAngle(0.0f);
                Balls[i].setMovement({
                    350.0f * static_cast<float>(pow(-1, i)),
                    350.0f * static_cast<float>(pow(-1, i + 1))
                });
            } 
            
            if (Balls[i].isScoreTwo()) {
                currPlayerOneScore++;
                Balls[i].setPosition(ORIGIN);
                Balls[i].setAngle(0.0f);
                Balls[i].setMovement({
                    350.0f * static_cast<float>(pow(-1, i)),
                    350.0f * static_cast<float>(pow(-1, i + 1))
                });
            }
        }
        if (currPlayerTwoScore >= 10)
            gMenu = LOSE;
        if (currPlayerOneScore >= 10)
            gMenu = WIN;
    }

    // Music
    if (gMusicStatus == ON) UpdateMusicStream(HeavenlyAudio);
}

void render() {
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    if ( gMenu == MAINMENU ) {
        renderObject(&MenuBackground, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);
    } 

    else if ( gMenu == GAME ) {
        renderObject(&GameBackground, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);

        DrawText(TextFormat("%d", currPlayerOneScore), SCREEN_WIDTH / 4, 40, 60, BLACK);
        DrawText(TextFormat("%d", currPlayerTwoScore), (SCREEN_WIDTH * 3) / 4, 40, 60, BLACK);

        // Ball(s)
        for (int i = 0; i < currBallCount; i++) Balls[i].render();

        // Wall(s)
        for (int i = 0; i < NUMBER_OF_WALLS; i++) Walls[i].render();

        // Player(s)
        PlayerOne->render();
        PlayerTwo->render();
    }

    else if ( gMenu == WIN ) {
        renderObject(&WinScreen, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);
    }

    else if ( gMenu == LOSE ) {
        renderObject(&LoseScreen, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);
    }
    
    EndDrawing();
}

void shutdown() {
    // Music & Sound Effects
    UnloadMusicStream(HeavenlyAudio);
    CloseAudioDevice();

    // Textures
    UnloadTexture(MenuBackground);
    UnloadTexture(GameBackground);
    UnloadTexture(WinScreen);
    UnloadTexture(LoseScreen);

    // Close Game
    CloseWindow();
}

int main(void) {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}