#pragma once

#include <glm/glm.hpp>

namespace Forge
{
    using QuadVector = std::unordered_map<std::pair<int, glm::vec2>, std::pair<glm::vec3, glm::vec3>>;

    struct Voxel;

    class Chunk
    {
    public:
        void GenerateChunk();

        inline QuadVector GetTopVertices() const;
        inline QuadVector GetFrontVertices() const { return mRenderQuadsFront; }

    private:
        QuadVector mRenderQuadsTop;
        QuadVector mRenderQuadsFront;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsRSide;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsBack;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsLSide;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsBottom;

        std::vector<Voxel> mVoxels;
    };
}