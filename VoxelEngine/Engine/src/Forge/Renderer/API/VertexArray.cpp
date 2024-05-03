#include "pch.h"
#include "VertexArray.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLVertexArray.h"

namespace Forge
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: CORE_ASSERT(false, "Renderer API \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return CreateRef<GLVertexArray>();
        default:
        {
            CORE_ASSERT(false, "Unknown RenderAPI!");
            return nullptr;
        }
        }

        return nullptr;
    }
}