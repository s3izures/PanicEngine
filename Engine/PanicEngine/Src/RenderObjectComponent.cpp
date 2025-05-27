#include "Precompiled.h"
#include "RenderObjectComponent.h"
#include "SaveUtil.h"

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
    SaveUtil::ReadBool("CastShadow", mCastShadow, value);
}

void RenderObjectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
    SaveUtil::WriteBool("CastShadow", mCastShadow, doc, value);
}

bool RenderObjectComponent::CanCastShadow() const
{
    return mCastShadow;
}
