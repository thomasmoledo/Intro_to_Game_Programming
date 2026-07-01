#include "CS3113/loseScreen.h"

// Global Constants
float ACCELERATION_OF_GRAVITY = 981.0f;
Vector2 MOUSE_SCALE = { 1, 1 };

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels = {};

Effects *gEffects = nullptr;

Camera2D gCamera = { 0 };
ShaderProgram gShader;
Vector2 gLightPosition = { 0.0f, 0.0f };
float gPlayerHealthRatio = 1.0f;

Vector2 gMousePos = { 0.0f, 0.0f };

Music bgm;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    if (gCurrentScene) gCurrentScene->shutdown();
    gCurrentScene = scene;
    gCurrentScene->initialise();
    if (gCurrentScene->getState().player)
        gCamera.target = gCurrentScene->getState().player->getPosition();
}

void initialise()
{
    InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Les Chats Maléfiques");
    InitAudioDevice();
    bgm = LoadMusicStream("assets/bossMusic.mp3");
    SetMusicVolume(bgm, 0.33f);
    PlayMusicStream(bgm);

    gShader.load("shaders/vertex.glsl", "shaders/fragment.glsl");

    gLevels[MAIN_MENU] = new MainMenu(ORIGIN, "#ffffff");
    gLevels[PHASE_1] = new Phase1(ORIGIN, "#ffffff");
    gLevels[PHASE_2] = new Phase2(ORIGIN, "#ffffff");
    gLevels[PHASE_3] = new Phase3(ORIGIN, "#ffffff");
    gLevels[WIN_SCREEN] = new WinScreen(ORIGIN, "#ffffff");
    gLevels[LOSE_SCREEN] = new LoseScreen(ORIGIN, "#ffffff");

    switchToScene(gLevels[MAIN_MENU]);
    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    gEffects = new Effects(ORIGIN, (float) SCREEN_SIZE.x * 1.5f, (float) SCREEN_SIZE.y * 1.5f);
    gEffects->start(FADEIN);
    gEffects->setEffectSpeed(2.0f);

    SetTargetFPS(FPS);
}

void processInput()
{
    gMousePos = GetMousePosition();

    if (gCurrentScene == gLevels[MAIN_MENU])
    {
        if (isColliding(gCurrentScene->getState().buttons[0].getPosition(),
            gCurrentScene->getState().buttons[0].getScale(), &gMousePos, &MOUSE_SCALE)
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(gCurrentScene->getState().clickSound);
            switchToScene(gLevels[PHASE_1]);
        }

        else if (isColliding(gCurrentScene->getState().buttons[1].getPosition(),
            gCurrentScene->getState().buttons[1].getScale(), &gMousePos, &MOUSE_SCALE)
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gCurrentScene->showControls();

        else if (isColliding(gCurrentScene->getState().buttons[2].getPosition(),
            gCurrentScene->getState().buttons[2].getScale(), &gMousePos, &MOUSE_SCALE)
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gAppStatus = TERMINATED;

        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) gCurrentScene->hideControls();
    }
        
    else if ((gCurrentScene == gLevels[WIN_SCREEN] ||
                gCurrentScene == gLevels[LOSE_SCREEN]) &&
                IsKeyPressed(KEY_ENTER))
        {
            PlaySound(gCurrentScene->getState().clickSound);
            switchToScene(gLevels[MAIN_MENU]);
        }

    else if (gCurrentScene == gLevels[WIN_SCREEN] || gCurrentScene == gLevels[LOSE_SCREEN])
    {
        if (IsKeyPressed(KEY_ENTER)) switchToScene(gLevels[MAIN_MENU]);
    }

    else if (gCurrentScene->getState().player)
    {
        gCurrentScene->getState().player->resetMovement();
        if (!gCurrentScene->getState().player->isFlying())
            gCurrentScene->getState().player->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

        if      (IsKeyDown(KEY_A)) gCurrentScene->getState().player->moveLeft();
        else if (IsKeyDown(KEY_D)) gCurrentScene->getState().player->moveRight();
        if      (IsKeyDown(KEY_SPACE)) gCurrentScene->getState().player->moveUp(ACCELERATION_OF_GRAVITY);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) gCurrentScene->getState().player->shoot(GetMousePosition());
    }

    // Switching Scenes
    if      (IsKeyPressed(KEY_ONE)) switchToScene(gLevels[PHASE_1]);
    else if (IsKeyPressed(KEY_TWO)) switchToScene(gLevels[PHASE_2]);
    else if (IsKeyPressed(KEY_THREE)) switchToScene(gLevels[PHASE_3]);

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    UpdateMusicStream(bgm);

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);

        if (gCurrentScene->getState().player)
        {
            Vector2 cameraTarget = { gCurrentScene->getState().player->getPosition().x,
                                    gCurrentScene->getState().player->getPosition().y };
            
            gEffects->update(FIXED_TIMESTEP, &cameraTarget);

            gLightPosition = gCurrentScene->getState().player->getPosition();
            gPlayerHealthRatio = gCurrentScene->getState().player->getCurrHealth() / 
                gCurrentScene->getState().player->getMaxHealth();

            panCamera(&gCamera, &cameraTarget);
        }
        else gEffects->update(FIXED_TIMESTEP, nullptr);

        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();

    ClearBackground(ColorFromHex(gCurrentScene->getBGColourHexCode()));
    gCurrentScene->renderBackground();

    BeginMode2D(gCamera);
    gShader.begin();
    gShader.setVector2("lightPosition", gLightPosition);
    if (gCurrentScene->getState().player)
        gShader.setFloat("playerHealthRatio", gPlayerHealthRatio);
    else gShader.setFloat("playerHealthRatio", 1.0f);

    gCurrentScene->render();

    gShader.end();
    gEffects->render();
    EndMode2D();

    gCurrentScene->renderUI();

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;
    gLevels.clear();

    delete gEffects;
    gEffects = nullptr;

    gShader.unload();

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