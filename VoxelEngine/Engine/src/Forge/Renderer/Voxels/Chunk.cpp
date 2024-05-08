#include "pch.h"
#include "Chunk.h"

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
        glm::vec3 addOffsetTL = glm::vec3(-0.5f, 0.0f, -0.5f);
        glm::vec3 addOffsetBR = glm::vec3(0.5f, 0.0f, 0.5f);

        QuadSpace topQuad = { currentPos + addOffsetTL, currentPos + addOffsetBR };
        std::shared_ptr<QuadSpace> frontQuad = nullptr;

        for (int idx = 0; idx < areaSize; ++idx)
        {
            int linedIndex = (idx + 1);
            const uint32_t maxHeight = 40 + ChunkHeights[idx];
            for (int column = 0; column <= maxHeight; ++column)
            {
                const uint32_t voxelIndex = (idx * 256) + column;
                mVoxels[voxelIndex].Type = (uint8_t)VoxelType::Dirt;

                //Ignore empty right side
                if ((linedIndex % 15 != 0 || idx == 0) && ChunkHeights[idx + 1] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 1);
                }
                //Ignore empty left side
                if ((linedIndex % 16 != 0) && ChunkHeights[idx - 1] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 3);
                }
                //Ignore empty top side
                if ((linedIndex > 15) && ChunkHeights[idx - 16] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 0);

                    if (frontQuad)
                    {
                        QuadKey key = { (int)frontQuad->StartPos.y, glm::vec3(frontQuad->EndPos.z,
                                                                              frontQuad->EndPos.y,
                                                                              frontQuad->EndPos.y) };
                        auto it = mRenderQuadsFront.find(key);
                        if (it != mRenderQuadsFront.end() && it->second.EndPos.x == frontQuad->EndPos.x - 1)
                        {
                            //it->second.EndPos = topQuad.EndPos;
                        }
                        else
                        {
                            mRenderQuadsFront.insert({ key, *frontQuad });
                        }
                        frontQuad = nullptr;
                    }
                }
                else if (!frontQuad)
                {
                    int zValue = (idx - (idx % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(idx % 16, column - 40, zValue);
                    QuadSpace newFrontQuad = { newPos, newPos };
                    frontQuad = std::make_unique<QuadSpace>(newFrontQuad);
                }
                else
                {
                    ++frontQuad->EndPos.y;
                }
                //Ignore empty bottom side
                if ((idx < areaSize - RowNum) && ChunkHeights[idx + 16] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 2);
                }
            }

            if (frontQuad)
            {
                QuadKey key = { (int)frontQuad->StartPos.y, glm::vec3(frontQuad->EndPos.z, frontQuad->EndPos.y, frontQuad->EndPos.z) };
                auto it = mRenderQuadsFront.find(key);
                if (it != mRenderQuadsFront.end())
                {
                    if (it->second.EndPos.x == topQuad.EndPos.x - 1)
                    {
                        //it->second.EndPos = topQuad.EndPos;
                    }
                    else
                    {
                        //mRenderQuadsTop.insert({ key, *frontQuad });
                    }
                }
                else
                {
                    //mRenderQuadsTop.insert({ key, *frontQuad });
                }
                frontQuad = nullptr;
            }

            if (idx == 79)
            {
                idx = 79;
            }
            //Ignore empty right side
            if ((linedIndex % 15 != 0 || idx == 0) && ChunkHeights[idx + 1] == ChunkHeights[idx])
            {
                ++topQuad.EndPos.x;
            }
            else if (idx > 15)
            {
                QuadKey key = { (int)topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z - 1) };
                auto it = mRenderQuadsTop.find(key);
                if (it != mRenderQuadsTop.end())
                {
                    QuadKey newKey = { (int)topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                    QuadSpace newQuad = { it->second.StartPos, topQuad.EndPos };
                    mRenderQuadsTop.insert({ newKey, newQuad });
                    mRenderQuadsTop.erase(key);
                }
                else
                {
                    key = { (int)topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                    mRenderQuadsTop.insert({ key, topQuad });
                }

                if (idx < areaSize - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(nextBlock % 16, ChunkHeights[nextBlock], zValue);
                    topQuad = { newPos + addOffsetTL, newPos + addOffsetBR };
                }
            }
            else
            {
                QuadKey key = { (int)topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                mRenderQuadsTop.insert({ key, topQuad });
                if (idx < areaSize - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % 16)) / 16;
                    glm::vec3 newPos = glm::vec3(nextBlock % 16, ChunkHeights[nextBlock], zValue);
                    topQuad = { newPos + addOffsetTL, newPos + addOffsetBR };
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
                {topVertex.EndPos.StartPos.x, topVertex.EndPos.StartPos.y, topVertex.EndPos.EndPos.z},
                {topVertex.EndPos.EndPos.x, topVertex.EndPos.StartPos.y, topVertex.EndPos.EndPos.z},
                {topVertex.EndPos.EndPos.x, topVertex.EndPos.StartPos.y, topVertex.EndPos.StartPos.z},
                {topVertex.EndPos.StartPos.x, topVertex.EndPos.StartPos.y, topVertex.EndPos.StartPos.z}
            };

            vector.push_back(vertices);
        }*/

        std::vector<QuadSpecs> vertices;

        for (auto& topVertex : mRenderQuadsTop)
        {
            glm::vec3 distance = { topVertex.second.EndPos.x - topVertex.second.StartPos.x,
                                   0,
                                   topVertex.second.EndPos.z - topVertex.second.StartPos.z };

            glm::vec3 center = { topVertex.second.StartPos.x + (distance.x / 2),
                                 topVertex.second.StartPos.y,
                                 topVertex.second.StartPos.z + (distance.z / 2) };

            //glm::mat4 matrix =  glm::translate(glm::mat4(1.0f), center);

            vertices.push_back({ distance, center });
        }

        return vertices;
    }
}