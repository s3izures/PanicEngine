#pragma once
#include "PhysicsObject.h"

namespace PanicEngine::Physics
{
    class CollisionShape;

    class RigidBody final : public PhysicsObject
    {
    public:
        RigidBody() = default;
        ~RigidBody() override;

        void Initialize(PanicEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f, bool addToWorld = true);
        void Terminate();

        void SetPosition(const PanicEngine::Math::Vector3& position);
        void SetVelocity(const PanicEngine::Math::Vector3& velocity);

        void Activate();
        void Deactivate();

        bool IsDynamic() const;

    private:
        void SyncWithGraphics() override;
        btRigidBody* GetRigidBody() override;
        btDefaultMotionState* mMotionState = nullptr;
        float mMass = 0.0f;

        PanicEngine::Graphics::Transform* mGraphicsTransform = nullptr;
        btRigidBody* mRigidBody = nullptr;
    };
}