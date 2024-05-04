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
                }
                //Ignore empty bottom side
                if ((idx < areaSize - RowNum) && ChunkHeights[idx + 16] >= ChunkHeights[idx])
                {
                    mVoxels[voxelIndex].Colliders |= (1 << 2);
                }
            }
        }
    }
}