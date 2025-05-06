#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

enum class CustomComponentId
{
    CustomDebugDraw = static_cast<uint32_t>(PanicEngine::ComponentId::Count)
    //New components here
};

enum class CustomServiceId
{
    CustomDebugDrawDisplay = static_cast<uint32_t>(PanicEngine::ServiceId::Count)
    //New services here
};