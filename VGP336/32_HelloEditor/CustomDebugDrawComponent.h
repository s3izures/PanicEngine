#pragma once
#include "CustomTypeId.h"

class CustomDebugDrawComponent : public PanicEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw() const;

private:
    const PanicEngine::TransformComponent* mTransformComponent = nullptr;
    PanicEngine::Math::Vector3 mPosition = PanicEngine::Math::Vector3::Zero;
    PanicEngine::Color mColor = PanicEngine::Colors::White;

    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0.0f;
};