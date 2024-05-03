#pragma once
#include "NotVoxel/Core/Application.h"

#ifdef VOXEL_PLATFORM_WINDOWS

extern VoxelForge::Application* VoxelForge::CreateApplication(VoxelForge::AppCommandLineArgs args);

int main(int argc, char** argv)
{
	VoxelForge::Log::Initialize();

	PROFILE_BEGIN_SESSION("Startup", "Tracing/NotProfile-Startup.json");
	auto app = VoxelForge::CreateApplication({ argc, argv });
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "Tracing/NotProfile-Runtime.json");
	app->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Shutdown", "Tracing/NotProfile-Shutdown.json");
	delete app;
	PROFILE_END_SESSION();

	return 0;
}

#else
	#error This Engine only supports Windows.
#endif