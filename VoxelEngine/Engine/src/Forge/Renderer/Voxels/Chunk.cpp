#include "pch.h"
#include "Chunk.h"

#include "Voxel.h"

namespace Forge
{
    static const int ChunkSize = 16 * 256 * 16;
    static const int RowNum = 16;

    static int ChunkHeights[ChunkSize] = {
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
        std::pair<glm::vec3, glm::vec3> topQuad = { currentPos, currentPos };
        std::shared_ptr<std::pair<glm::vec3, glm::vec3>> frontQuad = nullptr;

        for (int idx = 0; idx < areaSize; ++idx)
        {
            const uint32_t maxHeight = 40 + ChunkHeights[idx];
            for (int column = 0; column <= maxHeight; ++column)
            {
                const uint32_t voxelIndex = (idx * 256) + column;
                mVoxels[voxelIndex].Type = (uint8_t)VoxelType::Dirt;

                //Ignore empty right side
                if ((idx % 15 != 0 || idx == 0) && ChunkHeights[idx + 1] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 1);
                }
                //Ignore empty left side
                if ((idx % 16 != 0) && ChunkHeights[idx - 1] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 3);
                }
                //Ignore empty top side
                if ((idx > 15) && ChunkHeights[idx - 16] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 0);

                    if (frontQuad)
                    {
                        auto key = std::make_pair((int)frontQuad->first.y, glm::vec2(frontQuad->second.z, frontQuad->second.y));
                        auto it = mRenderQuadsFront.find(key);
                        if (it != mRenderQuadsTop.end() && it->second.second.x == topQuad.second.x - 1)
                        {
                            it->second.second = topQuad.second;
                        }
                        else
                        {
                            mRenderQuadsTop.insert({ key, *frontQuad });
                        }
                        frontQuad = nullptr;
                    }
                }
                else if (!frontQuad)
                {
                    int zValue = (idx - (idx % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(idx % 16, column - 40, zValue);
                    std::pair<glm::vec3, glm::vec3> topQuad = { newPos, newPos };
                    frontQuad = std::make_unique<std::pair<glm::vec3, glm::vec3>>(topQuad);
                }
                else
                {
                    ++frontQuad->second.y;
                }
                //Ignore empty bottom side
                if ((idx < areaSize - RowNum) && ChunkHeights[idx + 16] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 2);
                }
            }

            if (frontQuad)
            {
                auto key = std::make_pair((int)frontQuad->first.y, glm::vec2(frontQuad->second.z, frontQuad->second.y));
                auto it = mRenderQuadsFront.find(key);
                if (it != mRenderQuadsTop.end() && it->second.second.x == topQuad.second.x - 1)
                {
                    it->second.second = topQuad.second;
                }
                else
                {
                    mRenderQuadsTop.insert({ key, *frontQuad });
                }
                frontQuad = nullptr;
            }

            //Ignore empty right side
            if ((idx % 15 != 0 || idx == 0) && ChunkHeights[idx + 1] == ChunkHeights[idx])
            {
                ++topQuad.second.x;
            }
            else if (idx > 15)
            {
                auto key = std::make_pair((int)topQuad.first.x, glm::vec2(topQuad.second.x, topQuad.second.y));
                auto it = mRenderQuadsTop.find(key);
                if (it != mRenderQuadsTop.end() && it->second.second.z == topQuad.second.z - 1)
                {
                    it->second.second = topQuad.second;
                }
                else
                {
                    mRenderQuadsTop.insert({ key, topQuad });
                }

                if (idx < areaSize - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(nextBlock % 16, ChunkHeights[nextBlock], zValue);
                    std::pair<glm::vec3, glm::vec3> topQuad = { newPos, newPos };
                }
            }
            else
            {
                auto key = std::make_pair((int)topQuad.first.x, glm::vec2(topQuad.second.x, topQuad.second.y));
                mRenderQuadsTop.insert({ key, topQuad });
                if (idx < areaSize - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(nextBlock % 16, ChunkHeights[nextBlock], zValue);
                    std::pair<glm::vec3, glm::vec3> topQuad = { newPos, newPos };
                }
            }
        }
    }

    std::vector<QuadSpecs> Chunk::GetTopVertices()
    {
        /*std::vector<glm::vec3[4]> vector;

        for (auto& topVertex : mRenderQuadsTop)
        {
            glm::vec3 vertices[4] = {
                {topVertex.second.first.x, topVertex.second.first.y, topVertex.second.second.z},
                {topVertex.second.second.x, topVertex.second.first.y, topVertex.second.second.z},
                {topVertex.second.second.x, topVertex.second.first.y, topVertex.second.first.z},
                {topVertex.second.first.x, topVertex.second.first.y, topVertex.second.first.z}
            };

            vector.push_back(vertices);
        }*/

        std::vector<QuadSpecs> vertices;

        for (auto& topVertex : mRenderQuadsTop)
        {
            int DistanceX = topVertex.second.second.x - topVertex.second.first.x;
            int DistanceY = 0;
            int DistanceZ = topVertex.second.second.z - topVertex.second.first.z;

            glm::vec3 center = { topVertex.second.first.x + (DistanceX / 2), 
                                 topVertex.second.first.y,
                                 topVertex.second.first.z + (DistanceZ / 2) };

            vertices.push_back({ DistanceX , DistanceY, DistanceZ, center });
        }

        return vertices;
    }
}