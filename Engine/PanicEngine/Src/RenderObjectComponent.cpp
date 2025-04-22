#include "Precompiled.h"
#include "RenderObjectComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace PanicEngine;

void RenderObjectComponent::Initialize()
{
    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    ASSERT(renderService != nullptr, "RenderObjectComponent: render service not available");
    renderService->Register(this);
}

void RenderObjectComponent::Terminate()
{
    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    ASSERT(renderService != nullptr, "RenderObjectComponent: render service not available");
    renderService->Unregister(this);
}

void RenderObjectComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("CastShadow"))
    {
        mCastShadow = value["CastShadow"].GetBool();
    }
}

bool RenderObjectComponent::CanCastShadow() const
{
    return mCastShadow;
}
