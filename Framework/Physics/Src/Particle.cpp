#include "Precompiled.h"
#include "Particle.h"

using namespace PanicEngine;
using namespace PanicEngine::Physics;

void Particle::Initialize()
{
    mLifeTime = 0.0f;
    mCollisionShape.InitializeEmpty();
    mRigidbody.Initialize(mTransform, mCollisionShape, 1.0f, false);
}

void Particle::Terminate()
{
    mRigidbody.Terminate();
    mCollisionShape.Terminate();
}

void Particle::Activate(const ParticleInfo& info, const Math::Vector3& position, const Math::Vector3& velocity)
{
    mInfo = info;
    mLifeTime = info.lifeTime;
    if (mLifeTime > 0.0f)
    {
        mRigidbody.Activate();
    }
    mRigidbody.SetPosition(position);
    mRigidbody.SetVelocity(velocity);
}

void Particle::Update(float deltaTime)
{
    if (IsActive())
    {
        mLifeTime -= deltaTime;
        if (!IsActive())
        {
            mRigidbody.Deactivate();
        }
        else
        {
            float t = 1.0f - Math::Clamp(mLifeTime / mInfo.lifeTime, 0.0f, 1.0f);
            mTransform.scale = Math::Lerp(mInfo.startScale, mInfo.endScale, t);
        }
    }
}

bool Particle::IsActive() const
{
    return mLifeTime > 0.0f;
}

const Graphics::Transform& Particle::GetTransform() const
{
    return mTransform;
}

Color Particle::GetColor() const
{
    float t = 1.0f - Math::Clamp(mLifeTime / mInfo.lifeTime, 0.0f, 1.0f);
    return Math::Lerp(mInfo.startColor, mInfo.endColor, t);
}

const Math::Vector3& Particle::GetPosition() const
{
    return mTransform.position;
}
