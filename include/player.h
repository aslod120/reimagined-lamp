#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <math.h>
#include "environment.h"

typedef struct Character
{
    Vector2 position;
    Vector2 newPos; // used for calculating collision
    float size;
    float speed;
}Character;

Character playerInitialize(Vector2 startingPos, float size, float speed);
Vector2 playerSpawn(Character player);
void playerDraw(Character player, Color color);
Vector2 getDistance(float speed);
Character playerMove(Character player, float screenWidth, float screenHeight);

#endif