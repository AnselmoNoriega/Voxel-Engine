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

    struct Vec2Int
    {
        int32_t x = 0;
        int32_t z = 0;

        bool operator<(const Vec2Int& other) const
        {
            if (x == other.x)
            {
                return z < other.z;
            }
            return x < other.x;
        }

        Vec2Int operator* (int other) const
        {
            return {x * other, z * other };
        }
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
        ~Chunk();

        void GenerateChunk(Vec2Int position);
        void ConnectWithNeighbor(const std::map<Vec2Int, Ref<Chunk>>& neighborChunks);

        void Render();

    private:
        void SaveVertices(QuadKey& key, glm::vec3& startPos, glm::vec3 endPos, QuadVector& quadVector);
        void SetVertices(const QuadVector& quadVector, int idx);
        int GetPosition(uint16_t x, uint16_t y) const;

    private:
        Vec2Int mCoord;
        Vec2Int mPosition;

        std::vector<QuadSpecs> mQuadSpecs[6];
        std::vector<QuadSpecs> mQuadSpecsSides[6];

        std::vector<Voxel> mVoxels;

        int* mChunkHeights = nullptr;
    };
}