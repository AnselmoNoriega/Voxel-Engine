#pragma once

#include  <memory>

#include "PlatformDetection.h"

#ifdef VOXEL_DEBUG
	#if defined(VOXEL_PLATFORM_WINDOWS)
		#define DEBUGBREAK() __debugbreak()
	#elif defined(VOXEL_PLATFORM_LINUX)
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)
		#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define VOXEL_ENABLE_ASSERTS
#else
	#define DEBUGBREAK()
#endif

#define BIND_EVENT_FN(fn) [this](auto&&... args)-> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace VoxelForge
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "NotVoxel/Core/Log.h"
#include "NotVoxel/Core/Assert.h"