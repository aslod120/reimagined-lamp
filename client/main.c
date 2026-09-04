#include "engine.h"
#include <math.h>
#include "player.h"
#define MAX_ENV 20

int signNumf(float num);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    /*
    if(network_initialize() == 0)
    {
        return 1; // exit the program
    }
    */

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //engine_initialize();

    Character player = {0};
    player.position = (Vector2){screenWidth/2, screenHeight/2};
    player.size = 10;
    player.speed = 50;

    float deltaTime = 0;

    Rectangle environment[MAX_ENV] = {0};
    for(int i = 0; i < MAX_ENV; i++)
    {
        environment[i].height = GetRandomValue(10, 50);
        environment[i].width = GetRandomValue(10, 50);
        environment[i].x = GetRandomValue(0, screenWidth - environment[i].width);
        environment[i].y = GetRandomValue(0, screenHeight - environment[i].height);
    }

    int sign = 0;
    bool colliding = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        deltaTime = GetFrameTime();

        //engine_update();

        if(IsKeyDown(KEY_W)) // move up
        {
            player.newPos.y -= player.speed * deltaTime;
        }
        if(IsKeyDown(KEY_S)) // move down
        {
            player.newPos.y += player.speed * deltaTime;
        }
        if(IsKeyDown(KEY_D)) // move right
        {
            player.newPos.x += player.speed * deltaTime;
        }
        if(IsKeyDown(KEY_A)) // move left
        {
            player.newPos.x -= player.speed * deltaTime;

        }

        player.newPos.x = roundf(player.newPos.x);
        player.newPos.y = roundf(player.newPos.y);

        while(player.newPos.x != 0 || player.newPos.y != 0)
        {
            for(int i = 0; i < MAX_ENV; i++)
            {
                if(CheckCollisionRecs(environment[i], (Rectangle){(player.position.x - player.size/2) + signNumf(player.newPos.x), (player.position.y - player.size/2) + signNumf(player.newPos.y), player.size, player.size}))
                {
                    colliding = true;
                    break;
                }
            }
            if(colliding == false)
            {
                sign = signNumf(player.newPos.x);
                player.position.x += sign;
                player.newPos.x -= sign;
                sign = signNumf(player.newPos.y);
                player.position.y += sign;
                player.newPos.y -= sign;
            }
            else
            {
                player.newPos.x = 0;
                player.newPos.y = 0;
            }
            colliding = false;
        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            DrawRectangle(player.position.x - player.size/2,
                player.position.y - player.size/2, 
                player.size, 
                player.size, 
                WHITE);

            for(int i = 0; i < MAX_ENV; i++)
            {
                DrawRectangleRec(environment[i], GREEN);
            }

            //engine_draw();

        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //network_deinitialize();
    //--------------------------------------------------------------------------------------

    return 0;
}

int signNumf(float num)
{
    return (num > 0) - (num < 0);
}