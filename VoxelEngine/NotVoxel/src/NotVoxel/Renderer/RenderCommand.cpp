#include "pch.h"
#include "RenderCommand.h"

#include "Platform/RendererAPI/OpenGL/GLRendererAPI.h"

namespace VoxelForge
{
    Scope<RendererAPI> RenderCommand::sRendererAPI = CreateScope<GLRendererAPI>();
}