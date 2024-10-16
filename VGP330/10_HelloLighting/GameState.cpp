#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    Mesh mesh;
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";

    mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f,0.9f,1.0f };

    //Default is Skybox
    mesh = MeshBuilder::CreateSphere(100, 100, 1.0f);

    for (int i = 0; i < 1; i++)
    {
        RenderObject& planet = mPlanets.emplace_back();
        planet.meshBuffer.Initialize(mesh);
        planet.diffuseMapId = TextureCache::Get()->LoadTexture("filthy-space-panels-bl/filthy-space-panels_albedo.png");
        planet.normalMapId = TextureCache::Get()->LoadTexture("filthy-space-panels-bl/filthy-space-panels_normal-ogl.png");
        planet.specMapId = TextureCache::Get()->LoadTexture("filthy-space-panels-bl/filthy-space-panels_metallic.png");
        planet.bumpMapId = TextureCache::Get()->LoadTexture("filthy-space-panels-bl/filthy-space-panels_height.png");

        planet.transform.position.x = i;

        mPlanets.push_back(planet);
    }

    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    for (auto& planet : mPlanets)
    {
        planet.Terminate();
    }
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    mStandardEffect.Begin();
    for (auto& planet : mPlanets)
    {
        mStandardEffect.Render(planet);
    }
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
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.001f))
        {
            mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }
    if (ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit4("Ambient##Material", &mPlanets[0].material.ambient.r);
        ImGui::ColorEdit4("Diffuse##Material", &mPlanets[0].material.diffuse.r);
        ImGui::ColorEdit4("Specular##Material", &mPlanets[0].material.specular.r);
        ImGui::ColorEdit4("Emissive##Material", &mPlanets[0].material.emissive.r);
        ImGui::DragFloat("SpecPower", &mPlanets[0].material.power);
    }
    mStandardEffect.DebugUI();
    ImGui::End();
}
