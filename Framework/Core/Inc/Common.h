#pragma once
//include things required for windows

#define WIN32_LEAN_AND_MEAN //eliminates less frequently used APIs
#define NOMINMAX //no min max

// < > means not within project
// " " is within

//win32 headers
#include <objbase.h>
#include <windows.h>

//std headers
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>