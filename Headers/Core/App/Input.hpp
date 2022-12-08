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
        Core::Maths::Vec2 deltaMouse = Core::Maths::Vec2();
        Core::Maths::Vec2 MousePos = Core::Maths::Vec2();
        Core::Maths::IVec2 ScreenSize = Core::Maths::Vec2();
        char leftMouse = 0;
        char rightMouse = 0;
        bool mouseCaptured = false;
        float scroll = 0;
        char esc = 0;

        void UpdateInputs(GLFWwindow* window);
        void InitInputs(GLFWwindow* window, Core::Maths::IVec2 defaultSize);
        int GetLastKey();
        void ClearLastKey();
    };
}