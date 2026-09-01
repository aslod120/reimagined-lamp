#include "engine.h"

Vector2 mousePos = {0};
char data[MAX_DATA_LENGTH] = {0}; // data we receive
char buffer[MAX_DATA_LENGTH] = {0}; // data we are preparing to send out
int buffer_length = 0; // length of the data we are preparing to send out
Vector2 p2MousePos = {0};
GameState gameState = menu;

int errorState = 0;

int eventType = 0;


void engine_initialize()
{
    Vector2 mousePos = {0};
    char data[MAX_DATA_LENGTH] = {'\0'};
    Vector2 p2MousePos = {0};
    GameState gameState = menu;
    errorState = 0;
}

void engine_update()
{
    if(gameState == game)
    {
        mousePos = GetMousePosition();
        memcpy(buffer + buffer_length, &(mousePos.x), sizeof(float));
        buffer_length += sizeof(float);
        memcpy(buffer + buffer_length, &(mousePos.y), sizeof(float));
        buffer_length += sizeof(float);

        network_sendData(buffer, sizeof(float) * 2);
        buffer_length = 0;
        eventType = network_getEvents(data);
        if(eventType == 3)
        {
            memcpy(&(p2MousePos.x), data + buffer_length, sizeof(float));
            buffer_length += sizeof(float);
            memcpy(&(p2MousePos.y), data + buffer_length, sizeof(float));
            buffer_length += sizeof(float);
            buffer_length = 0;
        }        
    }
}

void engine_draw()
{
    if(gameState == menu)
    {
        if(drawButton((Rectangle){50, GetScreenHeight()/2, 160, 30}, "Become Client", GetFontDefault(), 20))
        {
            if(network_connect("127.0.0.1", 1234) == 1)
            {
                gameState = game;
            }
        }
        if(drawButton((Rectangle){50, GetScreenHeight()/2 - 60, 160, 30}, "Become Server", GetFontDefault(), 20))
        {
            if((errorState = network_createServer("127.0.0.1", 1234)) == 1)
            {
                gameState = game;
            }
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
}

void resetString(char *string)
{
    for(int i = 0; string[i] != '\0'; i++)
    {
        string[i] = '\0';
    }
}