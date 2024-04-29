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

#include "NotVoxel/Core/Core.h"
#include "NotVoxel/Core/Log.h"

#include "NotVoxel/Debug/Instrumentor.h"

#ifdef  VOXEL_PLATFORM_WINDOWS
#include <Windows.h>
#endif 