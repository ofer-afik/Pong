#pragma once

#include <raylib.h>
#include <iostream>

class Button
{
    public:
        Rectangle butRect;
        std::string label;
        Color bgColor;
        Color textColor;

        Button(float corner_x, float corner_y, float size_x, float size_y, std::string text, Color bgColor, Color textColor);

        bool isPressed();
        void Draw();
};