#pragma once

#include <glm/glm.hpp>

#include "Voxel.h"

namespace Forge
{

    struct QuadSpecs
    {
        glm::vec3 Distance;
        glm::vec3 Center;
    };

    struct QuadSpace
    {
        glm::vec3 StartPos;
        glm::vec3 EndPos;
    };

    struct QuadKey
    {
        int StartPos;
        glm::vec2 EndPos;

        bool operator<(const QuadKey& other) const
        {
            return StartPos < other.StartPos && EndPos.x < other.EndPos.x && EndPos.y < other.EndPos.y;
        }
    };

    using QuadVector = std::map<QuadKey, QuadSpace>;

    class Chunk
    {
    public:
        void GenerateChunk();

        std::vector<QuadSpecs> GetTopVertices();
        //inline QuadVector GetFrontVertices() const { return mRenderQuadsFront; }

    private:
        QuadVector mRenderQuadsTop;
        QuadVector mRenderQuadsFront;
        QuadVector mRenderQuadsRSide;
        QuadVector mRenderQuadsBack;
        QuadVector mRenderQuadsLSide;
        QuadVector mRenderQuadsBottom;

        std::vector<Voxel> mVoxels;
    };
}