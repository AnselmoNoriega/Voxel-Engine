#include "pch.h"
#include "VertexArray.h"

#include "NotVoxel/Renderer/Renderer.h"

#include "Platform/RendererAPI/OpenGL/GLVertexArray.h"

namespace VoxelForge
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
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