#pragma once

#include <glm/glm.hpp>

#include "Voxel.h"

namespace Forge
{

    struct QuadSpecs
    {
        glm::vec4 Distance;
        glm::mat4 Center;
    };

    struct QuadSpace
    {
        glm::vec3 StartPos;
        glm::vec3 EndPos;
    };

    struct QuadKey
    {
        int StartPos;
        glm::vec3 EndPos;

        bool operator<(const QuadKey& other) const
        {
            if (StartPos != other.StartPos)
            {
                return StartPos < other.StartPos;
            }
            if (EndPos.x != other.EndPos.x)
            {
                return EndPos.x < other.EndPos.x;
            }
            if (EndPos.y != other.EndPos.y)
            {
                return EndPos.y < other.EndPos.y;
            }
            return EndPos.z < other.EndPos.z;
        }
    };

    using QuadVector = std::map<QuadKey, QuadSpace>;

    class Chunk
    {
    public:
        void GenerateChunk();

        std::vector<QuadSpecs> GetTopVertices();
        std::vector<QuadSpecs> GetFrontVertices();

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