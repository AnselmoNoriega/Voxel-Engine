#include "pch.h"
#include "IndexBuffer.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLIndexBuffer.h"

namespace Forge
{
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: CORE_ASSERT(false, "RendererAPI \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return CreateRef<GLIndexBuffer>(indices, count);
        default:
        {
            CORE_ASSERT(false, "Unknown Renderer API!");
            return nullptr;
        }
        }
    }
}