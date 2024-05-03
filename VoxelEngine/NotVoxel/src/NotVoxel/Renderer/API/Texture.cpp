#include "pch.h"
#include "Texture.h"

#include "NotVoxel/Renderer/RendererAPI.h"
#include "NotVoxel/Renderer/Renderer.h"

#include "Platform/RendererAPI/OpenGL/GLTexture.h"

namespace VoxelForge
{
    Ref<Texture> Texture::Create(const TextureSpecification& specification)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: CORE_ASSERT(false, "Renderer API \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return CreateRef<GLTexture>(specification);
        default:
        {
            CORE_ASSERT(false, "Unknown RenderAPI!");
            return nullptr;
        }
        }
    }

    Ref<Texture> Texture::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None: CORE_ASSERT(false, "Renderer API \"None\" is currently not supported!");
        case RendererAPI::API::OpenGL: return CreateRef<GLTexture>(path);
        default:
        {
            CORE_ASSERT(false, "Unknown RenderAPI!");
            return nullptr;
        }
        }
    }
}