#pragma once
#include <cstdint>

namespace Forge
{
    enum class VoxelType : uint8_t
    {
        None,
        Dirt
    };

    struct Voxel
    {
        uint8_t Type = 0;
        uint8_t Colliders = 0;
    };
}