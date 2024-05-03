#pragma once

#include "Forge/Core/Core.h"
#include "Forge/Core/Input/KeyCodes.h"
#include "Forge/Core/Input/MouseCodes.h"

namespace Forge
{
    class  Input
    {
    public:
        static bool IsKeyPressed(KeyCode keycode);
        static bool IsMouseButtonPressed(MouseCode button);

        static std::pair<float, float> GetMousePosition();
    };
}