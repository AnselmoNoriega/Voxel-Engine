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
        static std::map<Vec2Int, Ref<Chunk>> mFarChunks;
        //static std::unordered_set<Vec2Int, pair_hash> mActiveCells;

        static int mMaxRenderDistanceSqrd;
    };

    //struct pair_hash 
    //{
    //    template <class T1, class T2>
    //    std::size_t operator() (const std::pair<T1, T2>& pair) const 
    //    {
    //        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    //    }
    //};
}

