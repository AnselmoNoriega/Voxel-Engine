#pragma once
#include <cstdint>

namespace Forge
{
    enum class VoxelType
    {
        None,
        Dirt
    };

    struct Voxel
    {
        VoxelType Type = VoxelType::None;
        uint8_t CollidersHeights[4];
    };
}