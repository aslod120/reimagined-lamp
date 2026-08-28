#ifndef UICONTROLS
#define UICONTROLS

#include "raylib.h"

// unselected UI color
#define DEFAULT ((Color){235, 64, 52, 255})
// mouse hovering color
#define HOVERING ((Color){166, 43, 35, 255})
// mouse clicking color
#define SELECTED ((Color){235, 113, 26, 255})
// standard color for the internal square for the checkbox
#define CHECKED GREEN

Color UI_stateToColor(int state); // translates button state to color
bool drawButton(Rectangle rect, const char *text, Font font, float size); // draws a functional button with text
bool drawCheckbox(const char *text, Vector2 position, float boxSize, bool isChecked, float thickness, Font font, float textSize); // draws a functional checkbox with text, returns the saved boolean

#endif