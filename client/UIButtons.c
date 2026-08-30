#include "UIButtons.h"

Color UI_stateToColor(int state)
{
    switch (state)
    {
    case 0:
        return DEFAULT;
        break;
    case 1:
        return HOVERING;
        break;
    case 2:
        return SELECTED;
        break;
    
    default:
        return DEFAULT;
        break;
    }
}

bool drawButton(Rectangle rect, const char *text, Font font, float size)
{
    bool result = false;
    int state = 0;

    if(CheckCollisionPointRec(GetMousePosition(), rect))
    {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){state = 2;}
        else {state = 1;}

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){result = true;}
    }

    // draw the button
    DrawRectangleRec(rect, UI_stateToColor(state));
    //DrawText(text, rect.x, rect.y, size, BLACK);
    DrawTextEx(font, text, (Vector2){rect.x, rect.y}, size, 2, WHITE);

    return result;
}

// check box
bool drawCheckbox(const char *text, Vector2 position, float boxSize, bool isChecked, float thickness, Font font, float textSize)
{
    int state = 0;
    Rectangle rect = {position.x, position.y, boxSize, boxSize};
    float margin = thickness + 5; // spacing between checkbox outline and square inside

    // checkbox interaction
    if(CheckCollisionPointRec(GetMousePosition(), rect))
    {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){state = 2;}
        else {state = 1;}

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){isChecked = !isChecked;}
    }

    // draw the checkbox square
    DrawRectangleLinesEx(rect, thickness, UI_stateToColor(state));
    // draw the inside portion of the checkbox 
    if(isChecked == true)
    {
        DrawRectangle(rect.x + margin, rect.y + margin, rect.width - margin * 2, rect.height - margin * 2, CHECKED);
    }
    //DrawText(text, rect.x + rect.width + thickness, rect.y + thickness, 30, RAYWHITE);
    DrawTextEx(font, text, (Vector2){rect.x + rect.width + thickness, rect.y + thickness}, textSize, 2, WHITE);

    return isChecked;
}