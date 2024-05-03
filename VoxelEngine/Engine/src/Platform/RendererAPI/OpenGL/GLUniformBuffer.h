#pragma once

#include "Forge/Renderer/API/UniformBuffer.h"

namespace Forge
{
	class GLUniformBuffer : public UniformBuffer
	{
	public:
		GLUniformBuffer(uint32_t size, uint32_t binding);
		~GLUniformBuffer() override;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t mRendererID = 0;
	};
}