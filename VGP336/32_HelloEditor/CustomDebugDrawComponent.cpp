#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"

using namespace PanicEngine;

void CustomDebugDrawComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
    CustomDebugDrawService* cdds = GetOwner().GetWorld().GetService<CustomDebugDrawService>();
    cdds->Register(this);
}

void CustomDebugDrawComponent::Terminate()
{
    CustomDebugDrawService* cdds = GetOwner().GetWorld().GetService<CustomDebugDrawService>();
    cdds->Unregister(this);
}

void CustomDebugDrawComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Slices"))
    {
        mSlices = static_cast<uint32_t>(value["Slices"].GetInt());
    }
    if (value.HasMember("Rings"))
    {
        mRings = static_cast<uint32_t>(value["Rings"].GetInt());
    }
    if (value.HasMember("Radius"))
    {
        mRadius = static_cast<uint32_t>(value["Radius"].GetFloat());
    }
    if (value.HasMember("Transform"))
    {
        auto pos = value["Transform"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
        mPosition.z = pos[2].GetFloat();
    }
    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        mColor.r = color[0].GetFloat();
        mColor.g = color[1].GetFloat();
        mColor.b = color[2].GetFloat();
        mColor.a = color[3].GetFloat();
    }
}

void CustomDebugDrawComponent::AddDebugDraw() const
{
    Math::Vector3 worldSpace = mPosition;
    if (mTransformComponent != nullptr)
    {
        worldSpace = TransformCoord(mPosition, mTransformComponent->GetMatrix4());
    }
    Graphics::SimpleDraw::AddSphere(mSlices, mRings, mRadius, worldSpace, mColor);
}
