#include "environment.h"

void shadowMapping(Vector2 *shadowPoints, int *num_shadows, Vector2 lightSource, Rectangle rect, float shadowDistance);

// box shadows
Vector2 shadowPoints[5] = {0};
int num_shadowPoints = 0;
Rectangle environment[MAX_ENV];

void environmentInitialize()
{
    // initialize boxes
    for(int i = 0; i < MAX_ENV; i++)
    {
        environment[i].height = GetRandomValue(10, 80);
        environment[i].width = GetRandomValue(10, 80);
        environment[i].x = GetRandomValue(0, GetScreenWidth() - environment[i].width);
        environment[i].y = GetRandomValue(0, GetScreenHeight() - environment[i].height);
    }
}

void environmentDraw(Vector2 player)
{
    // draw the environment
    for(int i = 0; i < MAX_ENV; i++)
    {
        //DrawRectangleRec(environment[i], GREEN);
        // make the shadow
        if(!IsKeyDown(KEY_SPACE))
        {
            shadowMapping(shadowPoints, &num_shadowPoints, player, environment[i], 300);
            DrawTriangleFan(shadowPoints, num_shadowPoints, BLACK);
        }
    }
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
