#include "engine.h"

Vector2 mousePos = {0};
char data[MAX_DATA_LENGTH] = {'\0'};
char *token = NULL;
Vector2 p2MousePos = {0};
GameState gameState = menu;

int errorState = 0;

int eventType = 0;

void engine_initialize()
{
    Vector2 mousePos = {0};
    char data[MAX_DATA_LENGTH] = {'\0'};
    char *token = NULL;
    Vector2 p2MousePos = {0};
    GameState gameState = menu;
    errorState = 0;
}

void engine_update()
{
    if(gameState == game)
    {
        mousePos = GetMousePosition();
        network_sendData(TextFormat("%.0f#%.0f", mousePos.x, mousePos.y));
        eventType = network_getEvents(data);
        if(eventType == 3)
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
        else if(eventType == 2)
        {
            if(network_serverOrClient() == -2)
            {
                network_disconnect();
                gameState = menu;
            }
        }
        
    }
}

void engine_draw()
{
    if(gameState == menu)
    {
        if(drawButton((Rectangle){50, GetScreenHeight()/2, 160, 30}, "Become Client", GetFontDefault(), 20))
        {
            if(network_connect("192.168.1.27", 1234) == 1)
            {
                gameState = game;
            }
        }
        if(drawButton((Rectangle){50, GetScreenHeight()/2 - 60, 160, 30}, "Become Server", GetFontDefault(), 20))
        {
            if((errorState = network_createServer("192.168.1.27", 1234)) == 1)
            {
                gameState = game;
            }
            DrawText(TextFormat("Value: %d", errorState), 0, 40, 20, RED);
        }
    }
    else if(gameState == game)
    {
        DrawRectangleV(mousePos, (Vector2){10,10}, RED);
        DrawRectangleV(p2MousePos, (Vector2){10,10}, BLUE);
        DrawText(TextFormat("%.0f:%.0f", p2MousePos.x, p2MousePos.y), 0, 20, 20, RED);
        //DrawText(TextFormat("Events: %u", network_getEventListLength()), 0, 40, 20, RED);
    
        if(drawButton((Rectangle){50, GetScreenHeight()/2, 160, 30}, "Disconnect", GetFontDefault(), 20))
        {
            network_disconnect();
            gameState = menu;
        }
    }
    DrawFPS(0,0);
    DrawText(TextFormat("Value: %d", network_serverOrClient()), 0, 60, 20, RED);
}

void resetString(char *string)
{
    for(int i = 0; string[i] != '\0'; i++)
    {
        string[i] = '\0';
    }
}