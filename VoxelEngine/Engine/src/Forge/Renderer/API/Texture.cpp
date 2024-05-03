#include "pch.h"
#include "Texture.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLTexture.h"

namespace Forge
{
    Ref<Texture> Texture::Create(const TextureSpecification& specification)
    {
        switch (RendererAPI::GetAPI())
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
        switch (RendererAPI::GetAPI())
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