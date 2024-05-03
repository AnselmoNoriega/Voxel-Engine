#include "pch.h"
#include "VertexBuffer.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLVertexBuffer.h"

namespace Forge
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: CORE_ASSERT(false, "RendererAPI \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return CreateRef<GLVertexBuffer>(size);
        default:
        {
            CORE_ASSERT(false, "Unknown Renderer API!");
            return nullptr;
        }
        }

        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: CORE_ASSERT(false, "RendererAPI \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return CreateRef<GLVertexBuffer>(vertices, size);
        default:
        {
            CORE_ASSERT(false, "Unknown Renderer API!");
            return nullptr;
        }
        }

        return nullptr;
    }
}
