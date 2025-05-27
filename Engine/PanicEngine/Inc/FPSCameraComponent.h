#pragma once
#include "Component.h"

namespace PanicEngine
{
    class CameraComponent;

    class FPSCameraComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::FPSCamera);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original) override;
        void Deserialize(const rapidjson::Value& value) override;

    private:
        CameraComponent* mCameraComponent = nullptr;
        float mShiftSpeed = 10.0f;
        float mMoveSpeed = 1.0f;
        float mTurnSpeed = 0.1f;
    };
}