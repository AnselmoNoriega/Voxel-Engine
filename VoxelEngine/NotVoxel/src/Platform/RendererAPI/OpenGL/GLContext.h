#pragma once

#include "NotVoxel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace VoxelForge
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

