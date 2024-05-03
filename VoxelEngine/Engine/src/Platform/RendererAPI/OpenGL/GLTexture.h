#pragma once

#include "Forge/Renderer/API/Texture.h"

namespace Forge
{
    class GLTexture : public Texture
    {
    public:
        GLTexture(const TextureSpecification& specification);
        GLTexture(const std::filesystem::path& path);
        ~GLTexture() override;

        const TextureSpecification& GetSpecification() const override { return mSpecification; }

        uint32_t GetWidth() const override { return mWidth; }
        uint32_t GetHeight() const override { return mHeight; }

        void SetData(void* data, uint32_t size) override;

        const std::filesystem::path& GetPath() const override { return mPath; }

        void Bind(uint32_t slot = 0) const override;
        uint32_t GetRendererID() override { return mID; };

        bool operator== (const Texture& other) const override
        {
            return mID == ((GLTexture&)other).mID;
        }

    private:
        TextureSpecification mSpecification;

        uint32_t mID;
        uint32_t mDataFormat;
        uint32_t mWidth, mHeight;
        std::filesystem::path mPath;
    };
}