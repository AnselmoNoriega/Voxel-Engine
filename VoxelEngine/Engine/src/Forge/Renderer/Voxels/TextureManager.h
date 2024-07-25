#pragma once
#include "Forge/Renderer/API/Texture.h"

namespace Forge
{
    struct QuadSpecs;
    enum class QuadPosition;

    class TextureManager
    {
    public:
        static void Initialize();

        static Ref<Texture> GetTexture(QuadPosition position);

    private:
        static std::map<int, std::array<Ref<Texture>, 3>> mTextures;
    };
}