#include "pch.h"
#include "Chunk.h"

#include "Forge/Renderer/Renderer.h"
#include "TextureManager.h"

namespace Forge
{
    static const int RowNum = 16;
    static const int MaxHeight = 216;
    static const int MinHeight = -40;
    static const int ColumnHeight = MaxHeight - MinHeight;
    static const int ChunkArea = RowNum * RowNum;
    static const int ChunkSize = ChunkArea * ColumnHeight;

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

    void Chunk::GenerateChunk(Vec2Int position)
    {
        mPosition = position;

        mVoxels.clear();
        mVoxels.resize(ChunkSize);
        QuadVector renderQuads[6];

        glm::vec3 currentPos = glm::vec3(0, ChunkHeights[0], 0);
        glm::vec3 frontLOffset = glm::vec3(-0.5f, 0.0f, -0.5f);
        glm::vec3 backROffset = glm::vec3(0.5f, 0.0f, 0.5f);

        QuadSpace topQuad = { currentPos + frontLOffset, currentPos + backROffset };

        for (int idx = 0; idx < ChunkArea; ++idx)
        {
            int linedIndex = (idx + 1);
            for (int column = MinHeight; column <= ChunkHeights[idx]; ++column)
            {
                const uint32_t voxelIndex = (idx * ColumnHeight) + column - MinHeight;
                mVoxels[voxelIndex].Type = (uint8_t)VoxelType::Dirt;

                //Check right side
                if ((linedIndex % RowNum != 0 || idx == 0) && ChunkHeights[idx + 1] >= column)
                {
                    mVoxels[voxelIndex].Colliders |= (1 << (int)QuadPosition::Right);
                }

                //Check left side
                if (idx % RowNum != 0 && ChunkHeights[idx - 1] >= column)
                {
                    mVoxels[voxelIndex].Colliders |= (1 << (int)QuadPosition::Left);
                }

                //Check front side
                if ((linedIndex > RowNum) && ChunkHeights[idx - RowNum] >= column)
                {
                    mVoxels[voxelIndex].Colliders |= (1 << (int)QuadPosition::Front);
                }

                //Check bottom side
                if ((idx < ChunkArea - RowNum) && ChunkHeights[idx + RowNum] >= column)
                {
                    mVoxels[voxelIndex].Colliders |= (1 << (int)QuadPosition::Back);
                }

                //Check top collision
                if (column != ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << (int)QuadPosition::Top);
                }

                //Check bottom collision
                if (column != MinHeight)
                {
                    mVoxels[voxelIndex].Colliders |= (1 << (int)QuadPosition::Bottom);
                }
            }

            //Check front side
            if (idx > 15 && ChunkHeights[idx - RowNum] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx - RowNum];
                float zValue = ((idx - (idx % RowNum)) / RowNum) - 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % RowNum) + 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % RowNum) - 0.5f, maxPos.y - height, zValue);

                glm::vec3 maxKeyPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, renderQuads[(int)QuadPosition::Front]);
            }

            //Check back side
            if ((idx < (ChunkArea - RowNum)) && ChunkHeights[idx + RowNum] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx + RowNum];
                float zValue = ((idx - (idx % RowNum)) / RowNum) + 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % RowNum) + 0.5f, maxPos.y - height, zValue);

                glm::vec3 minKeyPos = glm::vec3((idx % RowNum) - 0.5f, maxPos.y - height, zValue);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, renderQuads[(int)QuadPosition::Back]);
            }

            //Check right side
            if ((linedIndex % RowNum != 0 || idx == 0) && ChunkHeights[idx + 1] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx + 1];
                float zValue = ((idx - (idx % RowNum)) / RowNum);
                glm::vec3 maxPos = glm::vec3((idx % RowNum) + 0.5f, ChunkHeights[idx], zValue - 0.5f);
                glm::vec3 minPos = glm::vec3((idx % RowNum) + 0.5f, maxPos.y - height, zValue + 0.5f);

                glm::vec3 minKeyPos = glm::vec3((idx % RowNum) + 0.5f, maxPos.y - height, zValue - 1.5f);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, renderQuads[(int)QuadPosition::Right]);
            }

            //Check left side
            if (idx % RowNum != 0 && ChunkHeights[idx - 1] < ChunkHeights[idx])
            {
                int height = ChunkHeights[idx] - ChunkHeights[idx - 1];
                float zValue = ((idx - (idx % RowNum)) / RowNum);
                glm::vec3 maxPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue + 0.5f);
                glm::vec3 minPos = glm::vec3((idx % RowNum) - 0.5f, maxPos.y - height, zValue - 0.5f);

                glm::vec3 maxKeyPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue - 0.5f);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, renderQuads[(int)QuadPosition::Left]);
            }

            //Check top side
            if ((linedIndex % RowNum != 0) && ChunkHeights[idx + 1] == ChunkHeights[idx])
            {
                ++topQuad.EndPos.x;
            }
            else if (idx > 15)
            {
                QuadKey key = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z - 1) };
                auto it = renderQuads[(int)QuadPosition::Top].find(key);
                if (it != renderQuads[(int)QuadPosition::Top].end())
                {
                    QuadKey newKey = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                    QuadSpace newQuad = { it->second.StartPos, topQuad.EndPos };
                    renderQuads[(int)QuadPosition::Top].insert({ newKey, newQuad });
                    renderQuads[(int)QuadPosition::Top].erase(key);
                }
                else
                {
                    key = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                    renderQuads[(int)QuadPosition::Top].insert({ key, topQuad });
                }

                if (idx < ChunkArea - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % RowNum)) / RowNum;
                    glm::vec3 newPos = glm::vec3(nextBlock % RowNum, ChunkHeights[nextBlock], zValue);
                    topQuad = { newPos + frontLOffset, newPos + backROffset };
                }
            }
            else
            {
                QuadKey key = { topQuad.StartPos.x, glm::vec3(topQuad.EndPos.x, topQuad.EndPos.y, topQuad.EndPos.z) };
                renderQuads[(int)QuadPosition::Top].insert({ key, topQuad });
                if (idx < ChunkArea - 1)
                {
                    int nextBlock = idx + 1;
                    int zValue = (nextBlock - (nextBlock % RowNum)) / RowNum;
                    glm::vec3 newPos = glm::vec3(nextBlock % RowNum, ChunkHeights[nextBlock], zValue);
                    topQuad = { newPos + frontLOffset, newPos + backROffset };
                }
            }
        }

        //Check bottom side
        glm::vec3 startDownPos = glm::vec3(RowNum, MinHeight, 0);
        glm::vec3 endDownPos = glm::vec3(-1, MinHeight, RowNum - 1);
        QuadSpace bottomQuad = { startDownPos + frontLOffset, endDownPos + backROffset };
        QuadKey key = { bottomQuad.StartPos.x, glm::vec3(bottomQuad.EndPos.x, bottomQuad.EndPos.y, bottomQuad.EndPos.z) };
        renderQuads[(int)QuadPosition::Bottom].insert({ key, bottomQuad });

        for (uint8_t i = 0; i < 6; ++i)
        {
            SetVertices(renderQuads[i], i);
        }
    }

    void Chunk::ConnectWithNeighbor(Ref<Chunk>* neighborChunks)
    {
        QuadVector renderChunkSides[4];

        for (int idx = 0; idx < ChunkArea; ++idx)
        {
            int linedIndex = (idx + 1);

            if (auto& neighbor = neighborChunks[(int)QuadPosition::Front - 2]; neighbor && idx <= 15)
            {
                int height = ChunkHeights[idx] - neighbor->GetPosition(idx % RowNum, RowNum - 1);
                float zValue = ((idx - (idx % RowNum)) / RowNum) - 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % RowNum) + 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % RowNum) - 0.5f, maxPos.y - height, zValue);

                glm::vec3 maxKeyPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, renderChunkSides[(int)QuadPosition::Front - 2]);
            }

            if (auto& neighbor = neighborChunks[(int)QuadPosition::Back - 2]; neighbor && idx >= (ChunkArea - RowNum))
            {
                int height = ChunkHeights[idx] - neighbor->GetPosition(idx % RowNum, 0);
                float zValue = ((idx - (idx % RowNum)) / RowNum) + 0.5f;
                glm::vec3 maxPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue);
                glm::vec3 minPos = glm::vec3((idx % RowNum) + 0.5f, maxPos.y - height, zValue);

                glm::vec3 minKeyPos = glm::vec3((idx % RowNum) - 0.5f, maxPos.y - height, zValue);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, renderChunkSides[(int)QuadPosition::Back - 2]);
            }

            if (auto& neighbor = neighborChunks[(int)QuadPosition::Right - 2]; neighbor && linedIndex % RowNum == 0 && idx != 0)
            {
                int PositionY = (idx - (idx % RowNum)) / RowNum;
                int height = ChunkHeights[idx] - neighbor->GetPosition(0, PositionY);
                float zValue = ((idx - (idx % RowNum)) / RowNum);
                glm::vec3 maxPos = glm::vec3((idx % RowNum) + 0.5f, ChunkHeights[idx], zValue - 0.5f);
                glm::vec3 minPos = glm::vec3((idx % RowNum) + 0.5f, maxPos.y - height, zValue + 0.5f);

                glm::vec3 minKeyPos = glm::vec3((idx % RowNum) + 0.5f, maxPos.y - height, zValue - 1.5f);
                QuadKey key = { maxPos.y, minKeyPos };
                SaveVertices(key, maxPos, minPos, renderChunkSides[(int)QuadPosition::Right - 2]);
            }

            if (auto& neighbor = neighborChunks[(int)QuadPosition::Left - 2]; neighbor && idx % RowNum == 0)
            {
                int PositionY = (idx - (idx % RowNum)) / RowNum;
                int height = ChunkHeights[idx] - neighbor->GetPosition(RowNum - 1, PositionY);
                float zValue = ((idx - (idx % RowNum)) / RowNum);
                glm::vec3 maxPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue + 0.5f);
                glm::vec3 minPos = glm::vec3((idx % RowNum) - 0.5f, maxPos.y - height, zValue - 0.5f);

                glm::vec3 maxKeyPos = glm::vec3((idx % RowNum) - 0.5f, ChunkHeights[idx], zValue - 0.5f);
                QuadKey key = { minPos.y, maxKeyPos };
                SaveVertices(key, minPos, maxPos, renderChunkSides[(int)QuadPosition::Left - 2]);
            }
        }

        for (uint8_t i = 0; i < 4; ++i)
        {
            SetVertices(renderChunkSides[i], i + 2);
        }
    }


    void Chunk::SaveVertices(QuadKey& key, glm::vec3& startPos, glm::vec3 endPos, QuadVector& quadVector)
    {
        auto it = quadVector.find(key);
        if (it != quadVector.end())
        {
            QuadKey newKey = { startPos.y, endPos };
            QuadSpace newQuad = { it->second.StartPos, endPos };
            quadVector.insert({ newKey, newQuad });
            quadVector.erase(key);
        }
        else
        {
            key = { startPos.y, endPos };
            QuadSpace newFrontQuad = { startPos, endPos };
            quadVector.insert({ key, newFrontQuad });
        }
    }

    void Chunk::SetVertices(const QuadVector& quadVector, int idx)
    {
        for (auto& vertex : quadVector)
        {
            glm::vec3 distance = { vertex.second.EndPos.x - vertex.second.StartPos.x,
                                   vertex.second.EndPos.y - vertex.second.StartPos.y,
                                   vertex.second.EndPos.z - vertex.second.StartPos.z };

            glm::vec3 center = { vertex.second.StartPos.x + (distance.x / 2),
                                 vertex.second.StartPos.y + (distance.y / 2),
                                 vertex.second.StartPos.z + (distance.z / 2) };

            float with = distance.x ? distance.x : distance.z;
            float height = distance.y ? distance.y : distance.z;
            mQuadSpecs[idx].push_back({ distance, center, {with, height} });
        }
    }

    int Chunk::GetPosition(uint16_t x, uint16_t y)
    {
        return ChunkHeights[x + (y * RowNum)];
    }

    void Chunk::Render()
    {
        Ref<Texture> texturePtr = nullptr;

        for (uint8_t i = 0; i < 4; ++i)
        {
            for (const auto& quad : mQuadSpecs[i])
            {
                texturePtr = TextureManager::GetTexture(quad, (QuadPosition)i);
                Renderer::DrawFace(quad, texturePtr, { 1.0f, 1.0f, 1.0f, 1.0f });
            }
        }
        for (uint8_t i = 4; i < 6; ++i)
        {
            for (const auto& quad : mQuadSpecs[i])
            {
                texturePtr = TextureManager::GetTexture(quad, QuadPosition::Back);
                Renderer::DrawRLFace(quad, texturePtr, { 1.0f, 1.0f, 1.0f, 1.0f });
            }
        }
    }
}