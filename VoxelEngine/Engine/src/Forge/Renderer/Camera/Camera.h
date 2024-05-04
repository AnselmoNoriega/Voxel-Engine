#pragma once

#include "glm/glm.hpp"

namespace Forge
{
    class Camera
    {
    public:
        Camera() = default;

        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return mProjection; }

    protected:
        glm::mat4 mProjection = glm::mat4(1.0f);
    };
}