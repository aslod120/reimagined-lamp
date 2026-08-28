#include "raylib.h"
#include "network.h"
#include <stdlib.h>
#include <string.h>
#define MAX_DATA_LENGTH 80

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

void resetString(char *string)
{
    for(int i = 0; string[i] != '\0'; i++)
    {
        string[i] = '\0';
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    if(network_connect() != 0)
    {
        return 1;
    }

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Vector2 mousePos = {0};
    char data[MAX_DATA_LENGTH] = {'\0'};
    char *token = NULL;
    Vector2 p2MousePos = {0};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        mousePos = GetMousePosition();

        network_sendPacket(TextFormat("%.0f#%.0f", mousePos.x, mousePos.y));

        if(network_process(data))
        {
            token = strtok(data, "#");
            p2MousePos.x = atof(token);
            token = strtok(NULL, "#");
            p2MousePos.y = atof(token);
            while(token != NULL)
            {
                token = strtok(NULL, "#");
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangleV(mousePos, (Vector2){10,10}, RED);
            DrawRectangleV(p2MousePos, (Vector2){10,10}, BLUE);
            DrawText(TextFormat("%.0f:%.0f", p2MousePos.x, p2MousePos.y), 0, 20, 20, RED);

            DrawFPS(0,0);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    network_disconnect();
    network_deinitialize();
    //--------------------------------------------------------------------------------------

    return 0;
}