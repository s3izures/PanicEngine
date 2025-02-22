#include "Precompiled.h"
#include "Rigidbody.h"
#include "CollisionShape.h"
#include "PhysicsWorld.h"

using namespace PanicEngine;
using namespace PanicEngine::Physics;
using namespace PanicEngine::Graphics;

RigidBody::~RigidBody()
{
    ASSERT(mRigidBody == nullptr, "RigidBody: terminate must be called");
}

void RigidBody::Initialize(PanicEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass, bool addToWorld)
{
    mGraphicsTransform = &graphicsTransform;
    mMass = mass;

    mMotionState = new btDefaultMotionState(ConvertTobtTransform(graphicsTransform));
    mRigidBody = new btRigidBody(mMass, mMotionState, shape.mCollisionShape);
    mRigidBody->setRestitution(0.5f);
    if (addToWorld)
    {
        PhysicsWorld::Get()->Register(this);
    }
}

void RigidBody::Terminate()
{
    PhysicsWorld::Get()->Unregister(this);
    SafeDelete(mRigidBody);
    SafeDelete(mMotionState);
}

void RigidBody::SetPosition(const PanicEngine::Math::Vector3& position)
{
    mRigidBody->activate();
    mGraphicsTransform->position = position;
    mRigidBody->setWorldTransform(ConvertTobtTransform(*mGraphicsTransform));
}

void RigidBody::SetVelocity(const PanicEngine::Math::Vector3& velocity)
{
    mRigidBody->activate();
    mRigidBody->setLinearVelocity(TobtVector3(velocity));
}

void RigidBody::Activate()
{
    PhysicsWorld::Get()->Register(this);
    mRigidBody->activate();
}

void RigidBody::Deactivate()
{
    PhysicsWorld::Get()->Unregister(this);
}

bool RigidBody::IsDynamic() const
{
    return mMass > 0.0f;
}

void RigidBody::SyncWithGraphics()
{
    btTransform& worldTransform = mRigidBody->getWorldTransform();
    mGraphicsTransform->position = ToVector3(worldTransform.getOrigin());
    mGraphicsTransform->rotation = ToQuaternion(worldTransform.getRotation());
}

btRigidBody* RigidBody::GetRigidBody()
{
    return mRigidBody;
}
