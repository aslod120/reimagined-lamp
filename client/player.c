#include "player.h"

Vector2 getInput()
{
    Vector2 newPos = {0};
    float deltaTime = GetFrameTime();

    if(IsKeyDown(KEY_W)) // move up
    {
        newPos.y = -deltaTime;
    }
    if(IsKeyDown(KEY_S)) // move down
    {
        newPos.y = deltaTime;
    }
    if(IsKeyDown(KEY_D)) // move right
    {
        newPos.x = deltaTime;
    }
    if(IsKeyDown(KEY_A)) // move left
    {
        newPos.x = -deltaTime;
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