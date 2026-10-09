#include "engine.h"
#include <math.h>
#include "raymath.h"
#include "player.h"
#define MAX_ENV 100

int signNumf(float num);
void shadowMapping(Vector2 *shadowPoints, int *num_shadows, Vector2 lightSource, Rectangle rect, float shadowDistance);
void createVisionCone(Vector2 *points, Vector2 origin, Vector2 target, float angle, float distance);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;
    /*
    if(network_initialize() == 0)
    {
        return 1; // exit the program
    }
    */

    InitWindow(screenWidth, screenHeight, "Multiplayer Game");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //engine_initialize();

    Character player = {0};
    player.position = (Vector2){screenWidth/2, screenHeight/2};
    player.size = 10;
    player.speed = 50;

    float deltaTime = 0;
    Vector2 mousePos = {0};
    Vector2 mouseDir = {0};

    Rectangle environment[MAX_ENV] = {0};
    for(int i = 0; i < MAX_ENV; i++)
    {
        environment[i].height = GetRandomValue(10, 80);
        environment[i].width = GetRandomValue(10, 80);
        environment[i].x = GetRandomValue(0, screenWidth - environment[i].width);
        environment[i].y = GetRandomValue(0, screenHeight - environment[i].height);
    }

    Vector2 shadowPoints[5] = {0};
    int num_shadowPoints = 0;

    Vector2 visionPoints[3] = {0};
    float visionAngle = 70;
    float visionDistance = 1000;
    unsigned int triLoc[3] = {0};

    int sign = 0;
    bool xColliding = false;
    bool yColliding = false;

    bool playerColliding = false;
    // spawn the player in and make sure they aren't colliding with a rectangle when they do
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

    /*  example on how to set a uniform value in the shader
        unsigned int wLoc = GetShaderLocation(raytracing, "screenWidth");
        float sw = (float)GetScreenWidth();
        SetShaderValue(raytracing, wLoc, &sw, SHADER_UNIFORM_FLOAT);
    */

    // temp other player location and such
    Rectangle otherPlayer = {0};
    otherPlayer.width = 10;
    otherPlayer.height = 10;
    bool otherPlayerColliding = true;
    do
    {
        otherPlayerColliding = false;
        otherPlayer.x = GetRandomValue(0, screenWidth - otherPlayer.width);
        otherPlayer.y = GetRandomValue(0, screenHeight - otherPlayer.height);
        for(int i = 0; i < MAX_ENV; i++)
        {
            if(CheckCollisionRecs(environment[i], otherPlayer))
            {
                otherPlayerColliding = true;
                break;
            }
        }
    } while (otherPlayerColliding == true);

    // load the shader
    Shader shader = LoadShader(0, "./include/shaders/shader.fs");

    // get the shader value locations to test against the triangle
    for(int i = 0; i < 3; i++)
    {
        char triName[20] = "triangle[x]\0";
        triName[9] = '0' + i;
        triLoc[i] = GetShaderLocation(shader, triName);
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        deltaTime = GetFrameTime();
        mousePos = GetMousePosition();
        mouseDir = Vector2Normalize(Vector2Subtract(mousePos, player.position));

        //engine_update();

        player.newPos = getDistance(player.speed);

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

        visionPoints[0] = player.position;
        visionPoints[1] = Vector2Add(player.position, Vector2Rotate(Vector2Scale(mouseDir, visionDistance), (visionAngle/2) * DEG2RAD));
        visionPoints[2] = Vector2Add(player.position, Vector2Rotate(Vector2Scale(mouseDir, visionDistance), (-visionAngle/2) * DEG2RAD));
        //createVisionCone(visionPoints, player.position, GetMousePosition(), visionAngle, visionDistance);
        // send vision cone info to the shader
        
        for(int i = 0; i < 3; i++)
        {
            visionPoints[i].y = screenHeight - visionPoints[i].y;
            SetShaderValue(shader, triLoc[i], &visionPoints[i], SHADER_UNIFORM_VEC2);
        }
        

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

            //DrawTriangleFan(visionPoints, 3, WHITE);

            DrawRectangle(player.position.x - player.size/2,
                player.position.y - player.size/2, 
                player.size, 
                player.size, 
                BLUE);

            DrawRectangleRec(otherPlayer, RED);

            // draw the environment
            for(int i = 0; i < MAX_ENV; i++)
            {
                DrawRectangleRec(environment[i], GREEN);
                // make the shadow
                if(!IsKeyDown(KEY_SPACE))
                {
                    shadowMapping(shadowPoints, &num_shadowPoints, player.position, environment[i], 300);
                    DrawTriangleFan(shadowPoints, num_shadowPoints, BLACK);
                }
            }
            
            BeginShaderMode(shader);
            DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
            EndShaderMode();
            
            

            //engine_draw();

        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
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

// my convoluted way of making shadows from rectangles
void shadowMapping(Vector2 *shadowPoints, int *num_shadows, Vector2 lightSource, Rectangle rect, float shadowDistance)
{
    *num_shadows = 0;

    Vector2 point_1 = {rect.x, rect.y};
    Vector2 point_2 = {rect.x + rect.width, rect.y};
    Vector2 point_3 = {rect.x + rect.width, rect.y + rect.height};
    Vector2 point_4 = {rect.x, rect.y + rect.height};

    int position = 0;

    if(lightSource.y < rect.y)
    {
        if(lightSource.x < rect.x){position = 1;}
        else if(lightSource.x > rect.x + rect.width){position = 3;}
        else {position = 2;}
    }
    else if(lightSource.y > rect.y + rect.height)
    {
        if(lightSource.x < rect.x){position = 7;}
        else if(lightSource.x > rect.x + rect.width){position = 5;}
        else {position = 6;}
    }
    else
    {
        position = 4;
        if(lightSource.x < rect.x){position = 8;}
    }

    switch (position)
    {
    case 1:
        shadowPoints[*num_shadows] = point_2;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_1;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_4;
        (*num_shadows)++;
        break;
    case 2:
        shadowPoints[*num_shadows] = point_2;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_1;
        (*num_shadows)++;
        break;
    case 3:
        shadowPoints[*num_shadows] = point_3;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_2;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_1;
        (*num_shadows)++;        
        break;
    case 4:
        shadowPoints[*num_shadows] = point_3;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_2;
        (*num_shadows)++;
        break;
    case 5:        
        shadowPoints[*num_shadows] = point_4;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_3;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_2;
        (*num_shadows)++;
        break;
    case 6:
        shadowPoints[*num_shadows] = point_4;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_3;
        (*num_shadows)++;
        break;
    case 7:   
        shadowPoints[*num_shadows] = point_1;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_4;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_3;
        (*num_shadows)++;
        break;
    case 8:
        shadowPoints[*num_shadows] = point_1;
        (*num_shadows)++;
        shadowPoints[*num_shadows] = point_4;
        (*num_shadows)++;
        break;
    
    default:
        break;
    }

    Vector2 direction = {0};
    direction = Vector2Subtract(shadowPoints[(*num_shadows) - 1], lightSource);
    //direction = Vector2Normalize(direction);
    shadowPoints[*num_shadows] = Vector2Add(shadowPoints[(*num_shadows) - 1], Vector2Scale(direction, shadowDistance));
    (*num_shadows)++;
    
    direction = Vector2Subtract(shadowPoints[0], lightSource);
    //direction = Vector2Normalize(direction);
    shadowPoints[*num_shadows] = Vector2Add(shadowPoints[0], Vector2Scale(direction, shadowDistance));
    (*num_shadows)++;
    
}
