#include "pch.h"
#include "GraphicsContext.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLContext.h"

namespace Forge
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<GLContext>(static_cast<GLFWwindow*>(window));
		default:
		{
			CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
		}
	}
}