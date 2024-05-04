#pragma once

#include <glm/glm.hpp>

namespace Forge
{
    class Chunk
    {
    public:
        void GenerateChunk();

    private:
        std::vector<glm::vec3> renderQuads;
    };
}