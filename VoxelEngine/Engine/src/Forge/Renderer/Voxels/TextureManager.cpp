#include "pch.h"
#include "TextureManager.h"

#include "Chunk.h"

namespace Forge
{
    std::map<int, std::array<Ref<Texture>, 3>> TextureManager::mTextures;

    void TextureManager::Initialize()
    {
        mTextures.clear();
        std::array <Ref<Texture>, 3> texture = {
            Texture::Create("Assets/Textures/grass_block_top.png"),
            Texture::Create("Assets/Textures/grass_block_side.png"),
            Texture::Create("Assets/Textures/grass_block_side.png")
        };

        mTextures.insert({(int)VoxelType::Dirt, texture});
    }

    Ref<Texture> TextureManager::GetTexture(QuadPosition position)
    {
        if (position == QuadPosition::Top)
        {
            return mTextures[(int)VoxelType::Dirt][(int)QuadPosition::Top];
        }

        return mTextures[(int)VoxelType::Dirt][1];
    }
}