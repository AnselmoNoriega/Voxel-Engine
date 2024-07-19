#include "pch.h"
#include "World.h"

#include "TextureManager.h"

namespace Forge
{
    int World::mMaxRenderDistanceSqrd = 16 * 16;

    void World::Initialize()
    {
        TextureManager::Initialize();

        InitChunk(0, 0);
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
    }
}