#include "pch.h"
#include "GraphicsContext.h"

#include "NotVoxel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace VoxelForge
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWWindow*>(window));
		default:
		{
			CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
		}
	}
}