#ifndef ENGINE_H
#define ENGINE_H
#include "raylib.h"
#include "network.h"
#include <stdlib.h>
#include <string.h>
#include "UIButtons.h"
#define MAX_DATA_LENGTH 80

typedef enum GameState
{
    menu = 0,
    game = 1
}GameState;

void resetString(char *string);
void engine_initialize();
void engine_update();
void engine_draw();

#endif