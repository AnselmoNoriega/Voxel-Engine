#pragma once
#include "Forge/Core/Application.h"

#ifdef ENGINE_PLATFORM_WINDOWS

extern Forge::Application* Forge::CreateApplication(Forge::AppCommandLineArgs args);

int main(int argc, char** argv)
{
	Forge::Log::Initialize();

	PROFILE_BEGIN_SESSION("Startup", "Tracing/NotProfile-Startup.json");
	auto app = Forge::CreateApplication({ argc, argv });
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