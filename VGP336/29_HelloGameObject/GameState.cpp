#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    mGameWorld.Initialize();

    GameObject* transformGO = mGameWorld.CreateGameObject("Transform");
    TransformComponent* transformComponent = transformGO->AddComponent<TransformComponent>();

    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
    CameraComponent* cameraComponent = cameraGO->AddComponent<CameraComponent>();
    cameraGO->AddComponent<FPSCameraComponent>();
    cameraComponent->GetCamera().SetPosition({ 0.0f, 0.0f, -5.0f });
    cameraComponent->GetCamera().SetLookAt({ 0.0f, 0.0f, 0.0f });
    cameraGO->Initialize();

    mCameraComponent = cameraComponent;
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCameraComponent->GetCamera());
}
