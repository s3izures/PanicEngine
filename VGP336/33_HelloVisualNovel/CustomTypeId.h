#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

enum class CustomComponentId
{
    CustomDialogueComponent = static_cast<uint32_t>(PanicEngine::ComponentId::Count),
    //New components here
    CustomIncrementDialogueButtonComponent
};

enum class CustomServiceId
{
    CustomDialogueService = static_cast<uint32_t>(PanicEngine::ServiceId::Count)
    //New services here
};