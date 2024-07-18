#include "Precompiled.h"
#include "TimeUtil.h"

using namespace PanicEngine;
using namespace PanicEngine::Core;

float TimeUtil::GetTime()
{
    //gives duration since application started (run time)
    static const auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    return miliseconds / 1000.0f;
}

float TimeUtil::GetDeltaTime()
{
    //gives duration since application started (run time)
    static auto lastCallTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCallTime).count();
    lastCallTime = currentTime;
    return miliseconds / 1000.0f;
}
