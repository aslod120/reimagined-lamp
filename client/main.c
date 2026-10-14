#include "engine.h"
#include <math.h>
#include "raymath.h"
#include "player.h"
#include "environment.h"
#include "shooting.h"

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
    environmentInitialize();
    bulletInitialize(1, 8, 500);

    Character player = playerInitialize(10, (Vector2){screenWidth/2, screenHeight/2}, 10, 50);
    Vector2 newPlayerPos = {0}; // for passing info to the shader

    float deltaTime = 0;
    Vector2 mousePos = {0};
    Vector2 mouseDir = {0};

    // vision cone
    Vector2 visionPoints[3] = {0};
    float visionAngle = 70;
    float visionDistance = 1000;
    unsigned int triLoc[3] = {0};

    // spawn player
    player.position = playerSpawn(player);

    /*  example on how to set a uniform value in the shader
        unsigned int wLoc = GetShaderLocation(raytracing, "screenWidth");
        float sw = (float)GetScreenWidth();
        SetShaderValue(raytracing, wLoc, &sw, SHADER_UNIFORM_FLOAT);
    */

    // temp other player location and such
    Character otherPlayer = playerInitialize(10, (Vector2){0,0}, 10, 0);
    otherPlayer.position = playerSpawn(otherPlayer);

    // load the shader
    Shader shader = LoadShader(0, "./include/shaders/shader.fs");

    // get the shader value locations to test against the triangle vision cone
    for(int i = 0; i < 3; i++)
    {
        char triName[20] = "triangle[x]\0";
        triName[9] = '0' + i;
        triLoc[i] = GetShaderLocation(shader, triName);
    }

    // setup to pass the player position to the shader
    unsigned int playerPosLoc = GetShaderLocation(shader, "playerPos\0");

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
        player = playerMove(player, screenWidth, screenHeight);
        
        // get input to shoot
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            bulletSpawn(mouseDir, player.position);
        }
        bulletUpdate();
        
        // setup vision cone area
        visionPoints[0] = player.position;
        visionPoints[1] = Vector2Add(player.position, Vector2Rotate(Vector2Scale(mouseDir, visionDistance), (visionAngle/2) * DEG2RAD));
        visionPoints[2] = Vector2Add(player.position, Vector2Rotate(Vector2Scale(mouseDir, visionDistance), (-visionAngle/2) * DEG2RAD));

        // send vision cone area to the shader        
        for(int i = 0; i < 3; i++)
        {
            visionPoints[i].y = screenHeight - visionPoints[i].y;
            SetShaderValue(shader, triLoc[i], &visionPoints[i], SHADER_UNIFORM_VEC2);
        }
        // send the player position to the shader
        newPlayerPos = player.position;
        newPlayerPos.y = screenHeight - newPlayerPos.y;
        SetShaderValue(shader, playerPosLoc, &newPlayerPos, SHADER_UNIFORM_VEC2);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

            //DrawTriangleFan(visionPoints, 3, WHITE);

            // draw player
            playerDraw(player, BLUE);

            // draw the other players
            playerDraw(otherPlayer, RED);

            environmentDraw(player.position);
            bulletDraw(BLUE);
            
            
            // draw the outline
            BeginShaderMode(shader);
            DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
            EndShaderMode();

            // UI ---
            DrawText(TextFormat("bullets: %d", getNumBullets()), 0, 0, 20, GREEN);
            
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



