#include "player.h"

Vector2 getDistance(float speed)
{
    Vector2 newPos = {0};
    float deltaTime = GetFrameTime();

    if(IsKeyDown(KEY_W)) // move up
    {
        newPos.y -= speed * deltaTime;
    }
    if(IsKeyDown(KEY_S)) // move down
    {
        newPos.y += speed * deltaTime;
    }
    if(IsKeyDown(KEY_D)) // move right
    {
        newPos.x += speed * deltaTime;
    }
    if(IsKeyDown(KEY_A)) // move left
    {
        newPos.x -= speed * deltaTime;
    }

    return newPos;
}

int moveX(float amount)
{
    int xDistance = roundf(amount);
    while(xDistance != 0)
    {
        
    }
}