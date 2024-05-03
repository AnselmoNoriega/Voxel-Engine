#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Forge
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

#define CORE_TRACE(...) ::Forge::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  ::Forge::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  ::Forge::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::Forge::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::Forge::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define TRACE(...)      ::Forge::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)       ::Forge::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)       ::Forge::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)      ::Forge::Log::GetClientLogger()->error(__VA_ARGS__)
#define FATAL(...)      ::Forge::Log::GetClientLogger()->critical(__VA_ARGS__)