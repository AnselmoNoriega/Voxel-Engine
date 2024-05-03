#pragma once

#include "Forge/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Forge
{
    class GLContext : public GraphicsContext
    {
    public:
        GLContext(GLFWwindow* windowHandle);
        ~GLContext() override;

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* mWindowHandle;
    };
}

