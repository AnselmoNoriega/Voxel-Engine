#pragma once

#include "Forge/Renderer/API/IndexBuffer.h"

namespace Forge
{
    class GLIndexBuffer : public IndexBuffer
    {
    public:
        GLIndexBuffer(uint32_t* indices, uint32_t count);
        ~GLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        uint32_t GetCount() const override { return mCount; }

    private:
        uint32_t mID;
        uint32_t mCount;
    };
}