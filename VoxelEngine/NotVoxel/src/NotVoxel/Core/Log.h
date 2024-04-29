#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace NR
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

#define CORE_TRACE(...) ::NR::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  ::NR::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  ::NR::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::NR::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::NR::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define TRACE(...)      ::NR::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)       ::NR::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)       ::NR::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)      ::NR::Log::GetClientLogger()->error(__VA_ARGS__)
#define FATAL(...)      ::NR::Log::GetClientLogger()->critical(__VA_ARGS__)