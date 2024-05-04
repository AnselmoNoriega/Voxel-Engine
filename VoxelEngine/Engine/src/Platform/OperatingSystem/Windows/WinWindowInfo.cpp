#include "pch.h"
#include "Forge/Core/WindowInfo.h"

#include <GLFW/glfw3.h>

#include "Forge/Core/Application.h"

namespace Forge
{
    std::pair<int, int> WindowInfo::GetWindowSize()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
       int x, y; 
       glfwGetWindowSize(window, &x, &y);

       return { x, y };
    }
}