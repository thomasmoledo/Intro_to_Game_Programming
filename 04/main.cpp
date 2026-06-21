#include "CS3113/LevelC.h"

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Camera2D gCamera = { 0 };

Scene *gCurrentScene = nullptr;
std::map<SceneID, Scene*> gLevels = {};

Music bgm;

int currHealth = 3;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    gCurrentScene = scene;
    gCurrentScene->initialise();
    if (gCurrentScene->getState().protagonist)
        gCamera.target = gCurrentScene->getState().protagonist->getPosition();
    else gCamera.target = ORIGIN;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fruit Escape");
    InitAudioDevice();
    bgm = LoadMusicStream("assets/wiiMusic.mp3");
    SetMusicVolume(bgm, 0.33f);
    PlayMusicStream(bgm);

    gLevels[MAIN_MENU] = new MainMenu(ORIGIN, "#ffffff");
    gLevels[LEVEL_A] = new LevelA(ORIGIN, "#c2f0ed");
    gLevels[LEVEL_B] = new LevelB(ORIGIN, "#e0b6ae");
    gLevels[LEVEL_C] = new LevelC(ORIGIN, "#dca8e3");
    gLevels[WIN_SCREEN] = new WinScreen(ORIGIN, "#ffffff");
    gLevels[LOSE_SCREEN] = new LoseScreen(ORIGIN, "#ffffff");

    switchToScene(gLevels[MAIN_MENU]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{
    if (gCurrentScene == gLevels[MAIN_MENU] ||
        gCurrentScene == gLevels[WIN_SCREEN] ||
        gCurrentScene == gLevels[LOSE_SCREEN])
    {
        if (gCurrentScene == gLevels[MAIN_MENU] && IsKeyPressed(KEY_ENTER))
        {
            PlaySound(gCurrentScene->getState().clickSound);
            switchToScene(gLevels[LEVEL_A]);
        }

        else if ((gCurrentScene == gLevels[WIN_SCREEN] ||
                  gCurrentScene == gLevels[LOSE_SCREEN]) &&
                  IsKeyPressed(KEY_ENTER))
        {
            PlaySound(gCurrentScene->getState().clickSound);
            switchToScene(gLevels[MAIN_MENU]);
        }
    }

    else
    {        
        gCurrentScene->getState().protagonist->resetMovement();

        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().protagonist->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().protagonist->moveRight();

        if (IsKeyPressed(KEY_W) &&
            gCurrentScene->getState().protagonist->isCollidingBottom())
        {
            gCurrentScene->getState().protagonist->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (GetLength(gCurrentScene->getState().protagonist->getMovement()) > 1.0f)
            gCurrentScene->getState().protagonist->normaliseMovement();
    }

    // Scene switching input
    if      (IsKeyPressed(KEY_ONE)) switchToScene(gLevels[LEVEL_A]);
    else if (IsKeyPressed(KEY_TWO)) switchToScene(gLevels[LEVEL_B]);
    else if (IsKeyPressed(KEY_THREE)) switchToScene(gLevels[LEVEL_C]);

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        UpdateMusicStream(bgm);
        gCurrentScene->update(FIXED_TIMESTEP, &currHealth);
        deltaTime -= FIXED_TIMESTEP;

        if (gCurrentScene->getState().protagonist)
        {
            Vector2 currentPlayerPosition = { gCurrentScene->getState().protagonist->getPosition().x, ORIGIN.y };
            panCamera(&gCamera, &currentPlayerPosition);
        }

        // Display Player Position
        // float xPos = gCurrentScene->getState().protagonist->getPosition().x;
        // float yPos = gCurrentScene->getState().protagonist->getPosition().y;
    
        // std::string xPos2 = std::to_string(xPos);
        // std::string yPos2 = std::to_string(yPos); 
        
        // const char* xPosChar = xPos2.c_str();
        // const char* yPosChar = yPos2.c_str();
        // printf(xPosChar); printf("\t"); printf(yPosChar); printf("\n");
    }
}

void render()
{
    if (gCurrentScene == gLevels[MAIN_MENU] ||
        gCurrentScene == gLevels[WIN_SCREEN] ||
        gCurrentScene == gLevels[LOSE_SCREEN])
    {
        BeginDrawing();
        gCurrentScene->render();
        EndDrawing();
    }
    else
    {
        BeginDrawing();
        BeginMode2D(gCamera);

        gCurrentScene->render();

        EndMode2D();

        DrawText(TextFormat("%d", currHealth), gCamera.offset.x - (SCREEN_WIDTH / 2) + 20,
                gCamera.offset.y - (SCREEN_HEIGHT / 2) + 20, 50, BLACK);
        
        EndDrawing();
    }
}

void shutdown()
{
    for (std::pair<const SceneID, Scene*> &entry : gLevels)
        delete entry.second;

    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID != NO_SCENE)
        {
            SceneID id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}
