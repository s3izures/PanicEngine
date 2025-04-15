#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mGameObject.AddComponent<TransformComponent>();
    mGameObject.Initialize();
}

void GameState::Terminate()
{
    mGameObject.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    mGameObject.Update(deltaTime);
}

void GameState::Render()
{
    
}

void GameState::UpdateCamera(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::LBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameObject.DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCamera);
}
