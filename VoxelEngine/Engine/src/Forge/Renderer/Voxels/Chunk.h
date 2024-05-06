#pragma once

#include <glm/glm.hpp>

namespace Forge
{
    using QuadVector = std::unordered_map<std::pair<int, glm::vec2>, std::pair<glm::vec3, glm::vec3>>;

    struct Voxel;

    struct QuadSpecs
    {
        int DistanceX = 0;
        int DistanceY = 0;
        int DistanceZ = 0;

        glm::mat4 Center;
    };

    class Chunk
    {
    public:
        void GenerateChunk();

        std::vector<QuadSpecs> GetTopVertices();
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