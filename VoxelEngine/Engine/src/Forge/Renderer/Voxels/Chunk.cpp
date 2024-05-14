#include "pch.h"
#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Forge
{
    static const int ChunkSize = 16 * 256 * 16;
    static const int ChunkArea = 16 * 16;
    static const int RowNum = 16;

    static int ChunkHeights[ChunkArea] = {
         7,  6,  5,  7, -5,  6, -4, -4, -3,  1,  4,  6, -7,  3,  1,  0,
         3,  5,  7,  2,  8,  0,  2,  3, -8, -6,  7,  5,  1,  0,  4,  7,
        -1, -5,  0, -6, -1, -5, -1,  4, -8, -3,  2,  2, -9,  6,  3, -8,
         7,  6,  4,  8, -2,  2,  8, -6,  7,  2, -9, -5, -2, -8, -4,  4,
         2,  0,  7, -3, -9, -9, -9,  9,  2,  8,  5,  8, -3, -9, -4,  7,
        -1,  2,  0, -1, -2,  4,  5,  2,  4, -9, -1,  1,  4, -4,  3,  7,
         8, -9, -5,  1, -9,  0,  4,  6, -8, -3, -8, -7,  6,  8, -3,  0,
         2,  0, -8,  7,  3, -1,  3, -4,  7, -4,  8, -9,  9, -9, -6,  0,
        -1,  5,  7, -7, -8, -1, -1,  0,  5,  7,  1,  4,  0, -9, -4, -7,
         6,  7,  2, -4,  6,  8, -6, -7, -8, -2,  1, -7, -4, -2, -4,  2,
         1,  4, -6,  1,  2,  1, -9, -7, -8, -4,  7,  6,  2, -8,  1, -8,
        -5, -6, -5, -3,  1,  0,  4,  7,  0,  4, -2, -4,  1,  8, -4, -4,
        -1, -3,  4, -5,  5,  7, -3,  5,  4, -2, -1,  7,  0,  7,  3, -2,
         8, -4, -5, -5,  2,  3,  8,  1, -5, -2, -9, -3, -7,  0, -8, -2,
         6,  6, -8,  1, -4, -1, -7,  5, -6, -2, -2, -6, -3,  9,  1,  3,
         3,  1, -6,  9,  3, -3, -2,  1,  0,  5,  3, -4,  5,  5, -1,  3
    };

    void Chunk::GenerateChunk()
    {
        mVoxels.clear();
        mVoxels.resize(ChunkSize);
        const uint32_t areaSize = (RowNum * RowNum);

        glm::vec3 currentPos = glm::vec3(0, ChunkHeights[0], 0);
        glm::vec3 frontLOffset = glm::vec3(-0.5f, 0.0f, -0.5f);
        glm::vec3 backROffset = glm::vec3(0.5f, 0.0f, 0.5f);

        QuadSpace topQuad = { currentPos + frontLOffset, currentPos + backROffset };

        for (int idx = 0; idx < areaSize; ++idx)
        {
            int linedIndex = (idx + 1);
            const uint32_t maxHeight = 40 + ChunkHeights[idx];
            for (int column = 0; column <= maxHeight; ++column)
            {
                const uint32_t voxelIndex = (idx * 256) + column;
                mVoxels[voxelIndex].Type = (uint8_t)VoxelType::Dirt;

                //Ignore empty right side
                if ((linedIndex % 16 != 0 || idx == 0) && ChunkHeights[idx + 1] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 1);
                }
                //Ignore empty left side
                if ((linedIndex % 17 != 0) && ChunkHeights[idx - 1] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 3);
                }

                //Ignore empty top side
                if ((linedIndex > 16) && ChunkHeights[idx - 16] >= column - 40)
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 0);
                }

                //Ignore empty bottom side
                if ((idx < areaSize - RowNum - 1) && ChunkHeights[idx + 16] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 2);
                }
            }

            if (idx > 15 && ChunkHeights[idx - 16] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx - 16];
                float zValue = ((idx - (idx % 16)) / 16) - 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % 16) + 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % 16) - 0.5f, maxPos.y - height, zValue);

                glm::vec3 maxKeyPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, QuadPosition::Front);
            }
            else if(idx < 16)
            {
                float zValue = ((idx - (idx % 16)) / 16) - 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % 16) + 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % 16) - 0.5f, -40, zValue);

                glm::vec3 maxKeyPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, QuadPosition::Front);
            }

            if ((idx < (areaSize - RowNum - 1)) && ChunkHeights[idx + 16] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx + 16];
                float zValue = ((idx - (idx % 16)) / 16) + 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % 16) + 0.5f, maxPos.y - height, zValue);

                glm::vec3 minKeyPos = glm::vec3((idx % 16) - 0.5f, maxPos.y - height, zValue);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, QuadPosition::Back);
            }
            else if(idx >= (areaSize - RowNum))
            {
                float zValue = ((idx - (idx % 16)) / 16) + 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % 16) + 0.5f, -40, zValue);

                glm::vec3 minKeyPos = glm::vec3((idx % 16) - 0.5f, -40, zValue);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, QuadPosition::Back);
            }

            if ((linedIndex % 16 != 0 || idx == 0) && ChunkHeights[idx + 1] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx + 1];
                float zValue = ((idx - (idx % 16)) / 16);
                glm::vec3 maxPos = glm::vec3((idx % 16) + 0.5f, ChunkHeights[idx], zValue - 0.5f);
                glm::vec3 minPos = glm::vec3((idx % 16) + 0.5f, maxPos.y - height, zValue + 0.5f);

                glm::vec3 minKeyPos = glm::vec3((idx % 16) + 0.5f, maxPos.y - height, zValue - 1.5f);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, QuadPosition::Right);
            }
            else if(linedIndex % 16 == 0 && idx != 0)
            {
                float zValue = ((idx - (idx % 16)) / 16);
                glm::vec3 maxPos = glm::vec3((idx % 16) + 0.5f, ChunkHeights[idx], zValue - 0.5f);
                glm::vec3 minPos = glm::vec3((idx % 16) + 0.5f, -40, zValue + 0.5f);

                glm::vec3 minKeyPos = glm::vec3((idx % 16) + 0.5f, -40, zValue - 1.5f);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, QuadPosition::Right);
            }

            if (idx % 16 != 0 && ChunkHeights[idx - 1] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx - 1];
                float zValue = ((idx - (idx % 16)) / 16);
                glm::vec3 maxPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue + 0.5f);
                glm::vec3 minPos = glm::vec3((idx % 16) - 0.5f, maxPos.y - height, zValue - 0.5f);

                glm::vec3 maxKeyPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue - 0.5f);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, QuadPosition::Left);
            }
            else if(idx % 16 == 0)
            {
                float zValue = ((idx - (idx % 16)) / 16);
                glm::vec3 maxPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue + 0.5f);
                glm::vec3 minPos = glm::vec3((idx % 16) - 0.5f, -40, zValue - 0.5f);

                glm::vec3 maxKeyPos = glm::vec3((idx % 16) - 0.5f, ChunkHeights[idx], zValue - 0.5f);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, QuadPosition::Left);
            }

            if ((linedIndex % 16 != 0) && ChunkHeights[idx + 1] == ChunkHeights[idx])
            {
                ++topQuad.EndPos.x;
            }
            else if (idx > 15)
            {
                QuadKey key = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z - 1) };
                auto it = mRenderQuads[(int)QuadPosition::Top].find(key);
                if (it != mRenderQuads[(int)QuadPosition::Top].end())
                {
                    QuadKey newKey = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                    QuadSpace newQuad = { it->second.StartPos, topQuad.EndPos };
                    mRenderQuads[(int)QuadPosition::Top].insert({ newKey, newQuad });
                    mRenderQuads[(int)QuadPosition::Top].erase(key);
                }
                else
                {
                    key = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                    mRenderQuads[(int)QuadPosition::Top].insert({ key, topQuad });
                }

                if (idx < areaSize - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(nextBlock % 16, ChunkHeights[nextBlock], zValue);
                    topQuad = { newPos + frontLOffset, newPos + backROffset };
                }
            }
            else
            {
                QuadKey key = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                mRenderQuads[(int)QuadPosition::Top].insert({ key, topQuad });
                if (idx < areaSize - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(nextBlock % 16, ChunkHeights[nextBlock], zValue);
                    topQuad = { newPos + frontLOffset, newPos + backROffset };
                }
            }
        }

        glm::vec3 startDownPos = glm::vec3(RowNum, -40, 0);
        glm::vec3 endDownPos = glm::vec3(-1, -40, RowNum - 1);
        QuadSpace bottomQuad = { startDownPos + frontLOffset, endDownPos + backROffset };
        QuadKey key = { bottomQuad.StartPos.x, glm::vec3(bottomQuad.EndPos.x, bottomQuad.EndPos.y, bottomQuad.EndPos.z) };
        mRenderQuads[(int)QuadPosition::Bottom].insert({ key, bottomQuad });
    }


    void Chunk::SaveVertices(QuadKey& key, glm::vec3& startPos, glm::vec3 endPos, QuadPosition position)
    {
        int idx = (int)position;
        auto it = mRenderQuads[idx].find(key);
        if (it != mRenderQuads[idx].end())
        {
            QuadKey newKey = { startPos.y, endPos };
            QuadSpace newQuad = { it->second.StartPos, endPos };
            mRenderQuads[idx].insert({ newKey, newQuad });
            mRenderQuads[idx].erase(key);
        }
        else
        {
            key = { startPos.y, endPos };
            QuadSpace newFrontQuad = { startPos, endPos };
            mRenderQuads[idx].insert({ key, newFrontQuad });
        }
    }

    std::vector<QuadSpecs> Chunk::GetVertices(QuadPosition position)
    {
        std::vector<QuadSpecs> vertices;

        for (auto& vertex : mRenderQuads[(int)position])
        {
            glm::vec3 distance = { vertex.second.EndPos.x - vertex.second.StartPos.x,
                                   vertex.second.EndPos.y - vertex.second.StartPos.y,
                                   vertex.second.EndPos.z - vertex.second.StartPos.z };

            glm::vec3 center = { vertex.second.StartPos.x + (distance.x / 2),
                                 vertex.second.StartPos.y + (distance.y / 2),
                                 vertex.second.StartPos.z + (distance.z / 2) };

            float with = distance.x ? distance.x : distance.z;
            float height = distance.y ? distance.y : distance.z;
            vertices.push_back({ distance, center, { with, height } });
        }

        return vertices;
    }
}