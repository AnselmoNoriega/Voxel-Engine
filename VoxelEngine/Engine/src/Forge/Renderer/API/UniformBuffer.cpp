#include "pch.h"
#include "UniformBuffer.h"

#include "Forge/Renderer/RendererAPI.h"

#include "Platform/RendererAPI/OpenGL/GLUniformBuffer.h"

namespace Forge
{

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<GLUniformBuffer>(size, binding);
		default:
		{
			CORE_ASSERT(false, "Unknown RendererAPI!"); 
			return nullptr;
		}
		}

	}

}