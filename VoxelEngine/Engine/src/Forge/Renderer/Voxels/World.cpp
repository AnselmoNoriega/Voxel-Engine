#include "pch.h"
#include "World.h"

#include "TextureManager.h"

namespace Forge
{
    std::map<Vec2Int, Ref<Chunk>> World::mChunks;
    std::map<Vec2Int, Ref<Chunk>> World::mFarChunks;
    std::unordered_set<Vec2Int, pair_hash> World::mActiveCells;

    int World::mMaxRenderDistanceSqrd = 16 * 16;

    void World::Initialize()
    {
        TextureManager::Initialize();

        InitChunk(0, 0);
    }

    void World::Update(int newX, int newZ)
    {
        std::unordered_set<Vec2Int, pair_hash> newActiveCells;
        for (int x = newX - mMaxRenderDistanceSqrd; x <= newX + mMaxRenderDistanceSqrd; ++x)
        {
            for (int z = newZ - mMaxRenderDistanceSqrd; z <= newZ + mMaxRenderDistanceSqrd; ++z)
            {
                int magnitudFromStart = (x * x) + (z * z);
                if (magnitudFromStart <= mMaxRenderDistanceSqrd)
                {
                    newActiveCells.insert({ x, z });
                }
            }
        }

        for (const auto& cell : mActiveCells)
        {
            if (newActiveCells.find(cell) == newActiveCells.end())
            {
                mChunks.erase(cell); // Remove chunks that are no longer within the render distance
            }
        }

        mActiveCells = newActiveCells;

        for (const auto& cell : mActiveCells)
        {
            if (mChunks.find(cell) == mChunks.end())
            {
                InitChunk(cell.x, cell.z); // Create new chunks within the render distance
            }
        }
    }

    void World::InitChunk(int posX, int posZ)
    {
        int magnitudFromStart = (posX * posX) + (posZ * posZ);
        if (mMaxRenderDistanceSqrd < magnitudFromStart || mChunks.find({ posX, posZ }) != mChunks.end())
        {
            return;
        }

        mChunks.insert({ { posX, posZ }, CreateRef<Chunk>() });
        auto& chunk = mChunks.at({ posX, posZ });
        chunk->GenerateChunk({ posX, posZ });
        chunk->ConnectWithNeighbor(mChunks);

        //Front
        InitChunk(posX, posZ - 1);
        //Back
        InitChunk(posX, posZ + 1);
        //Right
        InitChunk(posX + 1, posZ);
        //Left
        InitChunk(posX - 1, posZ);

        mActiveCells.insert({posX, posZ});
    }
}