#pragma once
#include "Chunk.h"

namespace Forge
{
    class World
    {
    public:
        World();

    private:
        void InitChunk(int posX, int posZ);

    private:
        std::map<Vec2Int, Ref<Chunk>> mChunks;

        int mMaxRenderDistanceSqrd = 16 * 16;
    };
}

