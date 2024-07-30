#pragma once
#include "Chunk.h"

namespace Forge
{
    class World
    {
    public:
        static void Initialize();

        static void Update(int newX, int newZ);
        static void Render();

    private:
        static void InitChunk(int posX, int posZ);

    private:
        static std::map<Vec2Int, Ref<Chunk>> mChunks;
        //TODO static std::map<Vec2Int, Ref<Chunk>> mFarChunks;

        static int mMaxRenderDistanceSqrd;
    };
}

