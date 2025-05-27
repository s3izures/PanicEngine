#pragma once
#include "Component.h"

namespace PanicEngine
{
    class RigidBodyComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::RigidBody);

        void Initialize() override;
        void Terminate() override;
        void DebugUI() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original) override;
        void Deserialize(const rapidjson::Value& value) override;

        void SetPosition(const Math::Vector3& position);
        void SetVelocity(const Math::Vector3& velocity);
        
    private:
        friend class PhysicsService;
        Physics::CollisionShape mCollisionShape;
        Physics::RigidBody mRigidbody;
        float mMass = -1.0f;
    };
}