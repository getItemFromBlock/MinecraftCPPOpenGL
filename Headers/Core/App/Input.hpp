#pragma once

#include <GLFW/glfw3.h>

#include "Core/Maths/Maths.hpp"

#define INPUT_HOLD 1
#define INPUT_PRESS 2

namespace Core::App
{
    class Inputs
    {
    public:
        Core::Maths::Vec2D deltaMouse = Core::Maths::Vec2D();
        Core::Maths::Vec2D MousePos = Core::Maths::Vec2D();
        Core::Maths::Int2D ScreenSize = Core::Maths::Vec2D();
        char leftMouse = 0;
        char rightMouse = 0;
        bool mouseCaptured = false;
        float scroll = 0;
        char esc = 0;

        void UpdateInputs(GLFWwindow* window);
        void InitInputs(GLFWwindow* window, Core::Maths::Int2D defaultSize);
        int GetLastKey();
        void ClearLastKey();
    };
}