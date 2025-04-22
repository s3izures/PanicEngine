#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.AddService<RenderService>();
    mGameWorld.Initialize();

    //GameObject* transformGO = mGameWorld.CreateGameObject("Transform", L"../../Assets/templates/transform_obj.json");
    //transformGO->Initialize();

    GameObject* meshGO = mGameWorld.CreateGameObject("Mesh", L"../../Assets/templates/mesh_obj.json");
    meshGO->Initialize();

    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera", L"../../Assets/templates/fps_camera.json");
    cameraGO->Initialize();
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
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();
}
