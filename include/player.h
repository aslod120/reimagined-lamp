#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <math.h>

typedef struct Character
{
    Vector2 position;
    Vector2 newPos; // used for calculating collision
    float size;
    float speed;
}Character;

#endif