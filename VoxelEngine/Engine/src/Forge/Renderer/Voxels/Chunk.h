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
        void PushNewQuad(QuadKey& key, glm::vec3& startPos, glm::vec3 endPos, QuadVector& quadVector);
        void SaveQuads(const QuadVector& quadVector, int idx);

        int GetPosition(uint16_t x, uint16_t y) const;

    private:
        struct QuadVertex
        {
            glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
            glm::vec2 TexCoord = { 0.0f, 0.0f };
            glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 0.0f };
            float TexIndex = 0.0f;
        };

    private:
        Vec2Int mCoord;
        Vec2Int mPosition;

        std::vector<std::byte> mVertices;
        std::vector<Voxel> mVoxels;

        int* mChunkHeights = nullptr;
    };
}