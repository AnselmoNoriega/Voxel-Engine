#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "Forge/Core/Core.h"
#include "Forge/Core/Log.h"

#include "Forge/Debug/Instrumentor.h"

#ifdef  ENGINE_PLATFORM_WINDOWS
#include <Windows.h>
#endif 