#pragma once

#include "NotVoxel/Core/Core.h"
#include "NotVoxel/Core/Input/KeyCodes.h"
#include "NotVoxel/Core/Input/MouseCodes.h"

namespace VoxelForge
{
    class  Input
    {
    public:
        static bool IsKeyPressed(KeyCode keycode);
        static bool IsMouseButtonPressed(MouseCode button);

        static std::pair<float, float> GetMousePosition();
    };
}