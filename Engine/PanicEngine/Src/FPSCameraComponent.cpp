#include "Precompiled.h"
#include "FPSCameraComponent.h"

#include "CameraComponent.h"
#include "GameObject.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Input;
using namespace PanicEngine::Math;

void FPSCameraComponent::Initialize()
{
    mCameraComponent = GetOwner().GetComponent<CameraComponent>();
    ASSERT(mCameraComponent != nullptr, "FPSCameraComponent: camera was not found");
}

void FPSCameraComponent::Terminate()
{
    mCameraComponent = nullptr;
}

void FPSCameraComponent::Update(float deltaTime)
{
    Camera& camera = mCameraComponent->GetCamera();
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? mShiftSpeed : mMoveSpeed) * deltaTime;
    const float turnSpeed = mTurnSpeed * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        camera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        camera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        camera.Strafe(-moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::D))
    {
        camera.Strafe(moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        camera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        camera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::LBUTTON))
    {
        camera.Yaw(input->GetMouseMoveX() * turnSpeed);
        camera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}

void FPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("MoveSpeed"))
    {
        mMoveSpeed = value["MoveSpeed"].GetFloat();
    }
    if (value.HasMember("ShiftSpeed"))
    {
        mShiftSpeed = value["ShiftSpeed"].GetFloat();
    }
    if (value.HasMember("TurnSpeed"))
    {
        mTurnSpeed = value["TurnSpeed"].GetFloat();
    }
}
