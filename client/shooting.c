#include "shooting.h"

Bullet bullets[MAX_BULLETS] = {0};
int num_bullets = 0; // keep track of number of bullets in the scene

void bulletInitialize(float bulletDamage, float bulletSize, float baseSpeed)
{
    // setup standard parameters across bullets
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].rect.height = bulletSize;
        bullets[i].rect.width = bulletSize;
        bullets[i].damage = bulletDamage;
        bullets[i].speed = baseSpeed;
    }
}

// spawn a bullet
Bullet bulletSpawn(Vector2 direction, Vector2 spawnPoint)
{
    if(num_bullets < MAX_BULLETS)
    {
        bullets[num_bullets].direction = direction;
        bullets[num_bullets].rect.x = spawnPoint.x - bullets[num_bullets].rect.width/2;
        bullets[num_bullets].rect.y = spawnPoint.y - bullets[num_bullets].rect.height/2;
        num_bullets++;
    }
}

// id is range 0 - (MAX_BULLETS - 1)
void bulletDestroy(int id)
{
    if(id < 0 || id >= MAX_BULLETS)
    {
        return; // out of bounds
    }

    // swap the element to delete, with the last element
    num_bullets--;
    bullets[id] = bullets[num_bullets];
}

void bulletUpdate()
{
    float deltaTime = GetFrameTime();
    for(int i = 0; i < num_bullets; i++)
    {
        // update current location
        bullets[i].rect.x += bullets[i].direction.x * bullets[i].speed * deltaTime;
        bullets[i].rect.y += bullets[i].direction.y * bullets[i].speed * deltaTime;

        // detect environment collisions
        for(int j = 0; j < MAX_ENV; j++)
        {
            if(CheckCollisionRecs(bullets[i].rect, environment[j]) == true)
            {
                bulletDestroy(i);
                // move i back one because we just replaced that i was
                i--;
                // exit out of this loop
                break;
            }
        }

        // detect player detection
        // to do
        
    }
}

void bulletDraw(Color color)
{
    for(int i = 0; i < num_bullets; i++)
    {
        DrawRectangleRec(bullets[i].rect, color);
    }
}

int getNumBullets()
{
    return num_bullets;
}