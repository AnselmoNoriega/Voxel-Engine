#include "pch.h"
#include "RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLRendererAPI.h"

namespace Forge
{
    RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (sAPI)
        {
        case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateScope<GLRendererAPI>();
        default:
        {
            CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
        }
    }
}