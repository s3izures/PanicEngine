#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    MeshPX mesh;
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";

    mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    //Default is Skybox
    mesh = MeshBuilder::CreateSpherePX(30,30,1.0f);
    mPlanet.meshBuffer.Initialize(mesh);
    mPlanet.diffuseTexture.Initialize(L"../../Assets/Images/planets/earth/earth.jpg");
    mPlanet2.meshBuffer.Initialize(mesh);
    mPlanet2.diffuseTexture.Initialize(L"../../Assets/Images/planets/earth/earth.jpg");

    mPlanet.transform.position.x = -1.0f;
    mPlanet2.transform.position.x = 1.0f;

    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    mPlanet.Terminate();
    mPlanet2.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    mStandardEffect.SetCamera(mCamera);
}

void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mPlanet);
        mStandardEffect.Render(mPlanet2);
    mStandardEffect.End();
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
    
}
