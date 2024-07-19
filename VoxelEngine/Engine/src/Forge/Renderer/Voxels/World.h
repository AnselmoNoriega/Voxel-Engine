#pragma once
#include "Chunk.h"

namespace Forge
{
    class World
    {
    public:
        static void Initialize();

    private:
        static void InitChunk(int posX, int posZ);

    private:
        static std::map<Vec2Int, Ref<Chunk>> mChunks;

        static int mMaxRenderDistanceSqrd;
    };
}

