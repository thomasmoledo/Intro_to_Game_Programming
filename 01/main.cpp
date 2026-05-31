/**
* Author: Thomas Moledo
* Assignment: And I Do Make Art, Thank You Very Much
* Date due: [05/30/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>

enum Member { MURDOC, TWO_D, RUSSEL, NOODLE };

// Global Constants
constexpr int   SCREEN_WIDTH  = 1920,
                SCREEN_HEIGHT = 1080,
                FPS           = 60,
                SIZE          = 900;
constexpr float MAX_AMP       = 10.0f;

constexpr char    BG_COLOUR[] = "#ffffff";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 ORIGIN2     = { SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { 225, 225 };

// Background
Vector2 gBackgroundPos = ORIGIN,
        gBackgroundScale = { 1920, 1080 };
float gBackgroundAngle = 0.0f;

Music gAuraFarming;

// Global Variables
AppStatus gAppStatus     = RUNNING;
// Scene 1
float     gAngle1_1      = 90.0f,
          gAngle1_2      = 210,
          gAngle1_3      = 0,
          gPulseTime1    = 0.0f,
          gPulseTime2    = (2.0f * PI) / 3,
          gPulseTime3    = 2 * gPulseTime2,
          gPrevTicks     = 0.0f,
          ORBIT_DIST     = 450.f;
Vector2   gPosition1_1   = ORIGIN2,
          gPosition1_2   = ORIGIN2,
          gPosition1_3   = ORIGIN2;
Vector2   gScale         = BASE_SIZE;
int       gFrames        = 0;

// Scene 2
Vector2 gPosition2_1 = { ORIGIN2.x + 400, ORIGIN2.y + 400 },
        gPosition2_2 = ORIGIN2,
        gPosition2_3 = { ORIGIN2.x - 400, ORIGIN2.y - 400 };
float   gAngle2 = 0.0f;

// Scene 3
Vector2 gPosition3_1 = { ORIGIN.x - 210, ORIGIN.y },
        gPosition3_2 = ORIGIN,
        gPosition3_3 = { ORIGIN.x + 210, ORIGIN.y };

Texture2D gCat1,
          gCat2,
          gCat3,
          gDomainText,
          gBackground;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project1");
    InitAudioDevice();

    gCat1 = LoadTexture("assets/cat1.png");
    gCat2 = LoadTexture("assets/cat2.png");
    gCat3 = LoadTexture("assets/cat3.png");

    gAuraFarming = LoadMusicStream("assets/domainClash.mp3");
    gBackground = LoadTexture("assets/background.png");
    gDomainText = LoadTexture("assets/domainText.png");
    SetMusicVolume(gAuraFarming, 0.33f);
    PlayMusicStream(gAuraFarming);

    SetTargetFPS(FPS);
}

void processInput() { 
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    ++gFrames;
    float ticks = GetTime();
    float deltaTime = ticks - gPrevTicks;
    gPrevTicks = ticks;

    // Scene 1
    if (gFrames < 600) {
        ORBIT_DIST -= 20.0f * deltaTime;
        gPulseTime1 += 1.0f * deltaTime;
        gPulseTime2 += 1.0f * deltaTime;
        gPulseTime3 += 1.0f * deltaTime;
        gAngle1_1 += 50.0f * deltaTime;
        gAngle1_2 += 50.0f * deltaTime;
        gAngle1_3 += 50.0f * deltaTime;

        

        gPosition1_1.x = ORIGIN2.x + ORBIT_DIST * cos(gPulseTime1);
        gPosition1_1.y = ORIGIN2.y + ORBIT_DIST * sin(gPulseTime1);

        gPosition1_2.x = ORIGIN2.x + ORBIT_DIST * cos(gPulseTime2);
        gPosition1_2.y = ORIGIN2.y + ORBIT_DIST * sin(gPulseTime2);

        gPosition1_3.x = ORIGIN2.x + ORBIT_DIST * cos(gPulseTime3);
        gPosition1_3.y = ORIGIN2.y + ORBIT_DIST * sin(gPulseTime3);
    }

    // Scene 2
    else if (gFrames < 1100) {
        gPosition2_1.x -= 10.0f * deltaTime;
        gPosition2_1.y -= 10.0f * deltaTime;

        gPosition2_3.x += 10.0f * deltaTime;
        gPosition2_3.y += 10.0f * deltaTime;
    }

    // Scene 3
    else {
        if (gFrames < 1250) {
            gPosition3_1.x -= 155.0f * deltaTime;
            gPosition3_3.x += 155.0f * deltaTime;
        } else if (gFrames < 1300) {
            gScale = { gScale.x * 1.01f, gScale.y * 1.01f};
        }
    }
      
    UpdateMusicStream(gAuraFarming);
}

void render() {
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    // Scene 1
    if (gFrames < 600) {
        renderObject(&gBackground, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);
        renderObject(&gCat1, &gPosition1_1, &gScale, &gAngle1_1);
        if (gFrames > 150) renderObject(&gCat2, &gPosition1_2, &gScale, &gAngle1_2);
        if (gFrames > 300) renderObject(&gCat3, &gPosition1_3, &gScale, &gAngle1_3);
    }

    // Scene 2
    else if (gFrames < 1100) {
        renderObject(&gBackground, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);
        renderObject(&gCat1, &gPosition2_1, &gScale, &gAngle2);
        renderObject(&gCat2, &gPosition2_2, &gScale, &gAngle2);
        renderObject(&gCat3, &gPosition2_3, &gScale, &gAngle2);
    }

    // Scene 3
    else {
        renderObject(&gCat1, &gPosition3_1, &gScale, &gAngle2);
        renderObject(&gCat2, &gPosition3_2, &gScale, &gAngle2);
        renderObject(&gCat3, &gPosition3_3, &gScale, &gAngle2);
        if (gFrames > 1300) {
            renderObject(&gDomainText, &gBackgroundPos, &gBackgroundScale, &gBackgroundAngle);
        }
    }

    EndDrawing();
}

void shutdown() {
    UnloadTexture(gCat1);
    UnloadTexture(gCat2);
    UnloadTexture(gCat3);
    UnloadTexture(gDomainText);
    UnloadTexture(gBackground);
    UnloadMusicStream(gAuraFarming);
    CloseAudioDevice();
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