#include "player.h"

int signNumf(float num);

Character playerInitialize(Vector2 startingPos, float size, float speed)
{
    Character player = {0};
    player.position = startingPos;
    player.size = size;
    player.speed = speed;
    return player;
}

Vector2 playerSpawn(Character player)
{
    Vector2 spawnLoc = {0};
    // spawning player
    bool playerColliding = false;
    // spawn the player in and make sure they aren't colliding with a rectangle when they do
    do
    {
        playerColliding = false;
        spawnLoc.x = GetRandomValue(player.size/2, GetScreenWidth() - player.size/2);
        spawnLoc.y = GetRandomValue(player.size/2, GetScreenHeight() - player.size/2);
        for(int i = 0; i < MAX_ENV; i++)
        {
            if(CheckCollisionRecs(environment[i], (Rectangle){spawnLoc.x - player.size/2, spawnLoc.y - player.size/2, player.size, player.size}))
            {
                playerColliding = true;
                break;
            }
        }
    } while (playerColliding == true);
    return spawnLoc;
}

void playerDraw(Character player, Color color)
{
    DrawRectangle(player.position.x - player.size/2,
                player.position.y - player.size/2, 
                player.size, 
                player.size, 
                color);
}

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

Character playerMove(Character player, float screenWidth, float screenHeight)
{

    // collision
    int sign = 0;
    bool xColliding = false;
    bool yColliding = false;
    // get player input and where they will end up next
    player.newPos = getDistance(player.speed);
    // round that location to a whole number
    player.newPos.x = roundf(player.newPos.x);
    player.newPos.y = roundf(player.newPos.y);
    // step by step, test the collision for the player
    while(player.newPos.x != 0 || player.newPos.y != 0)
    {
        xColliding = false;
        yColliding = false;
        for(int i = 0; i < MAX_ENV; i++)
        {
            // check x collision
            if(CheckCollisionRecs(environment[i], (Rectangle){(player.position.x - player.size/2) + signNumf(player.newPos.x), (player.position.y - player.size/2), player.size, player.size}))
            {
                xColliding = true;
            }
            // check y collision
            if(CheckCollisionRecs(environment[i], (Rectangle){(player.position.x - player.size/2), (player.position.y - player.size/2) + signNumf(player.newPos.y), player.size, player.size}))
            {
                yColliding = true;
            }
        }
        if(xColliding == false)
        {
            sign = signNumf(player.newPos.x);
            player.position.x += sign;
            player.newPos.x -= sign;
        }
        if(yColliding == false)
        {
            sign = signNumf(player.newPos.y);
            player.position.y += sign;
            player.newPos.y -= sign;
        }
        if(xColliding == true)
        {
            player.newPos.x = 0;
        }
        if(yColliding == true)
        {
            player.newPos.y = 0;
        }
    }
    // more collision
    // player can't escape through the border of the screen
    if(player.position.x - player.size/2 < 0)
    {
        player.position.x = player.size/2;
    }
    else if(player.position.x + player.size/2 > screenWidth)
    {
        player.position.x = screenWidth - player.size/2;
    }
    if(player.position.y - player.size/2 < 0)
    {
        player.position.y = player.size/2;
    }
    else if(player.position.y + player.size/2 > screenHeight)
    {
        player.position.y = screenHeight - player.size/2;
    }

    return player;
}

int signNumf(float num)
{
    return (num > 0) - (num < 0);
}