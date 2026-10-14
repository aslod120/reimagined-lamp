#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <math.h>
#include "environment.h"

typedef struct Character
{
    unsigned int id; // to keep track of which player we are in multiplayer
    float health;
    Vector2 position;
    Vector2 newPos; // used for calculating collision
    float size;
    float speed;
}Character;

Character playerInitialize(float health, Vector2 startingPos, float size, float speed);
Vector2 playerSpawn(Character player);
void playerDraw(Character player, Color color);
Vector2 getDistance(float speed);
Character playerMove(Character player, float screenWidth, float screenHeight);
bool playerHurt(Character player, float damage);

#endif