#ifndef SHOOTING_H
#define SHOOTING_H

#include "raylib.h"
#include "environment.h"

#define MAX_BULLETS 50

typedef struct Bullet
{
    float damage;
    float speed;
    Vector2 direction;
    Rectangle rect;
}Bullet;

extern Bullet bullets[MAX_BULLETS];
void bulletInitialize(float bulletDamage, float bulletSize, float baseSpeed);
Bullet bulletSpawn(Vector2 direction, Vector2 spawnPoint);
void bulletDestroy(int id);
void bulletUpdate();
void bulletDraw(Color color);
int getNumBullets();


#endif