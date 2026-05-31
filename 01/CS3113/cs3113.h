#ifndef CS3113_H
#define CS3113_H
#define LOG(argument) std::cout << argument << '\n'

#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

enum AppStatus { TERMINATED, RUNNING };

Color ColorFromHex(const char *hex);
void Normalise(Vector2 *vector);
float GetLength(const Vector2 *vector);
void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale, float *angle);

#endif // CS3113_H