#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "raylib.h"
#include "raymath.h"

#define MAX_ENV 100
extern Rectangle environment[MAX_ENV];

void environmentInitialize();
void environmentDraw(Vector2 player);

#endif