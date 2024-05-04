#pragma once

#include <glm/glm.hpp>

namespace Forge
{
    struct Voxel;

    class Chunk
    {
    public:
        void GenerateChunk();

    private:
        std::vector<glm::vec3> mRenderQuads;
        std::vector<Voxel> mVoxels;
    };
}