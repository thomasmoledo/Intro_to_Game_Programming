#ifndef CS3113_H
#define CS3113_H
#define LOG(argument) std::cout << argument << '\n'

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <queue>

enum AppStatus   { TERMINATED, RUNNING         };
enum TextureType { SINGLE, ATLAS               };
enum SceneID     { NO_SCENE = -1, MAIN_MENU, PHASE_1, PHASE_2, PHASE_3, WIN_SCREEN, LOSE_SCREEN };

// Global Constants
constexpr int FPS = 60;

constexpr Vector2 SCREEN_SIZE = { 2560, 1440 },
                  ORIGIN = { SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Functions
Color     ColorFromHex(const char *hex);
void      Normalise(Vector2 *vector);
float     GetLength(const Vector2 vector);
Rectangle getUVRectangle(
    const Texture2D *texture, int index, int rows, int cols
);

void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale);

void panCamera(Camera2D *camera, const Vector2 *targetPosition);

bool isColliding(const Vector2 postionA,  const Vector2 scaleA, 
                 const Vector2 *positionB, const Vector2 *scaleB);

#endif // CS3113_H