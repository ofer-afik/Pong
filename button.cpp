#include <raylib.h>
#include "button.hpp"

Button::Button(float corner_x, float corner_y, float size_x, float size_y, std::string text, Color bgCol, Color textCol)
{
    butRect = Rectangle{corner_x, corner_y, size_x, size_y};
    label = text;
    bgColor = bgCol;
    textColor = textCol;
};

bool Button::isPressed()
{
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return false;
    }

    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, butRect))
    {
        return true;
    }
    return false;
}

void Button::Draw()
{
    DrawRectangleRounded(butRect, 0.25, 6, bgColor);
    DrawText(label.c_str(), butRect.x + 30, butRect.y + 20, 100, textColor);
}