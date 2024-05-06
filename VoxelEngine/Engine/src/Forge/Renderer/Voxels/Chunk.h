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
        std::unordered_map<std::pair<int, glm::vec2>, std::pair<glm::vec3, glm::vec3>> mRenderQuadsTop;
        std::unordered_map<std::pair<int, glm::vec2>, std::pair<glm::vec3, glm::vec3>> mRenderQuadsFront;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsRSide;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsBack;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsLSide;
        std::vector<std::pair<glm::vec3, glm::vec3>> mRenderQuadsBottom;

        std::vector<Voxel> mVoxels;
    };
}