#include "Precompiled.h"
#include "PhysicsService.h"

#include "RigidBodyComponent.h"

using namespace PanicEngine;

void PhysicsService::Update(float deltaTime)
{
    if (mEnabled)
    {
        Physics::PhysicsWorld::Get()->Update(deltaTime);
    }
}

void PhysicsService::DebugUI()
{
    if (mEnabled)
    {
        Physics::PhysicsWorld::Get()->DebugUI();
    }
}

void PhysicsService::Deserialize(const rapidjson::Value& value)
{
    Physics::PhysicsWorld::Settings settings = Physics::PhysicsWorld::Get()->GetSettings();
    if (value.HasMember("Gravity"))
    {
        const auto& gravity = value["Gravity"].GetArray();
        settings.gravity.x = gravity[0].GetFloat();
        settings.gravity.y = gravity[1].GetFloat();
        settings.gravity.z = gravity[2].GetFloat();
        Physics::PhysicsWorld::Get()->SetGravity(settings.gravity);
    }
    if (value.HasMember("SimSteps"))
    {
        settings.simulationSteps = value["SimSteps"].GetFloat();
    }
    if (value.HasMember("FixedSimSteps"))
    {
        settings.fixedTimeStep = value["FixedSimSteps"].GetFloat();
    }
    Physics::PhysicsWorld::Get()->UpdateSettings(settings);
}

void PhysicsService::Register(RigidBodyComponent* rigidBodyComponent)
{
    Physics::PhysicsWorld::Get()->Register(&rigidBodyComponent->mRigidbody);
}

void PhysicsService::Unregister(RigidBodyComponent* rigidBodyComponent)
{
    Physics::PhysicsWorld::Get()->Unregister(&rigidBodyComponent->mRigidbody);
}

void PhysicsService::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}
