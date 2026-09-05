#include "engine.h"
#include <math.h>
#include "player.h"
#define MAX_ENV 45

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
        environment[i].height = GetRandomValue(10, 80);
        environment[i].width = GetRandomValue(10, 80);
        environment[i].x = GetRandomValue(0, screenWidth - environment[i].width);
        environment[i].y = GetRandomValue(0, screenHeight - environment[i].height);
    }

    int sign = 0;
    bool xColliding = false;
    bool yColliding = false;

    bool playerColliding = false;
    do
    {
        playerColliding = false;
        player.position.x = GetRandomValue(player.size/2, screenWidth - player.size/2);
        player.position.y = GetRandomValue(player.size/2, screenHeight - player.size/2);
        for(int i = 0; i < MAX_ENV; i++)
        {
            if(CheckCollisionRecs(environment[i], (Rectangle){player.position.x - player.size/2, player.position.y - player.size/2, player.size, player.size}))
            {
                playerColliding = true;
                break;
            }
        }
    } while (playerColliding == true);

    // load the shader
    Shader shader = LoadShader(0, "./include/shaders/shader.fs");
    Image whiteImage = GenImageColor(screenWidth, screenHeight, WHITE);
    Texture2D dummyTex = LoadTextureFromImage(whiteImage);
    UnloadImage(whiteImage);

    /*  example on how to set a uniform value in the shader
        unsigned int wLoc = GetShaderLocation(raytracing, "screenWidth");
        float sw = (float)GetScreenWidth();
        SetShaderValue(raytracing, wLoc, &sw, SHADER_UNIFORM_FLOAT);
    */

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

            BeginShaderMode(shader);
            DrawTexture(dummyTex, 0, 0, WHITE);
            EndShaderMode();

            //engine_draw();

        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(dummyTex);
    UnloadShader(shader);
    CloseWindow();        // Close window and OpenGL context
    //network_deinitialize();
    //--------------------------------------------------------------------------------------

    return 0;
}

int signNumf(float num)
{
    return (num > 0) - (num < 0);
}