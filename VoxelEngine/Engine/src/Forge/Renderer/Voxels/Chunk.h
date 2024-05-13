#pragma once

#include <glm/glm.hpp>

#include "Voxel.h"

namespace Forge
{
    enum class QuadPosition
    {
        Top,
        Bottom,
        Front,
        Back,
        Left,
        Right
    };

    struct QuadSpecs
    {
        glm::vec3 Distance;
        glm::vec3 Center;
        glm::vec2 DistanceVec2;
    };

    struct QuadSpace
    {
        glm::vec3 StartPos;
        glm::vec3 EndPos;
    };

    struct QuadKey
    {
        float StartPos;
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
        std::vector<QuadSpecs> GetBottomVertices();
        std::vector<QuadSpecs> GetFrontVertices();
        std::vector<QuadSpecs> GetBackVertices();
        std::vector<QuadSpecs> GetRightVertices();
        std::vector<QuadSpecs> GetLeftVertices();

    private:
        void SaveVertices(QuadKey& key, glm::vec3& startPos, glm::vec3 endPos, QuadPosition position);

    private:
        QuadVector mRenderQuads[6];
        QuadVector mRenderQuadsTop;
        QuadVector mRenderQuadsFront;
        QuadVector mRenderQuadsBack;
        QuadVector mRenderQuadsRight;
        QuadVector mRenderQuadsLeft;
        QuadVector mRenderQuadsBottom;

        std::vector<Voxel> mVoxels;
    };
}