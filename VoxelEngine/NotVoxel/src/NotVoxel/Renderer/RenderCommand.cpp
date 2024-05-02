#include "pch.h"
#include "RenderCommand.h"

#include "Platform/RenderAPI/OpenGL/GLRendererAPI.h"

namespace VoxelForge
{
    Scope<RendererAPI> RenderCommand::sRendererAPI = CreateScope<GLRendererAPI>();
}