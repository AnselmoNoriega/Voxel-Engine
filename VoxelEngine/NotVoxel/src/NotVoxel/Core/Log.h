#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace VoxelForge
{
	using loggerPtr = std::shared_ptr<spdlog::logger>;

	class  Log
	{
	public:
		static void Initialize();

		inline static loggerPtr& GetCoreLogger() { return sCoreLogger; }
		inline static loggerPtr& GetClientLogger() { return sClientLogger; }

	private:
		static loggerPtr sCoreLogger;
		static loggerPtr sClientLogger;
	};
}

#define CORE_TRACE(...) ::VoxelForge::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  ::VoxelForge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  ::VoxelForge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::VoxelForge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::VoxelForge::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define TRACE(...)      ::VoxelForge::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)       ::VoxelForge::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)       ::VoxelForge::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)      ::VoxelForge::Log::GetClientLogger()->error(__VA_ARGS__)
#define FATAL(...)      ::VoxelForge::Log::GetClientLogger()->critical(__VA_ARGS__)