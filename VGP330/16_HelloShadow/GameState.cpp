#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

const char* gCharas[] =
{
    "Prisoner",
    "Amy"
};

void GameState::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    std::filesystem::path shaderFilePP = L"../../Assets/Shaders/PostProcessing.fx";

    mCamera.SetPosition({ 0.0f,1.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f,0.9f,1.0f };

    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    mCharacter1.Initialize(L"../../Assets/Models/Prisoner/Prisoner.model");
    mCharacter1.Initialize(L"../../Assets/Models/Amy/Amy.model");

    MeshPX screenQuad = MeshBuilder::CreateScreenQuad();

    Mesh groundMesh = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    Mesh shape2 = MeshBuilder::CreateSphere(10,10,0.5f);
    mShape[1].meshBuffer.Initialize(shape2);
    mShape[1].transform.position.x = 1.0f;
    mShape[1].transform.position.y = 0.6f;
}

void GameState::Terminate()
{
    mShape[1].Terminate();
    mGround.Terminate();
    mCharacter1.Terminate();
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

int currentRenderWorld = 0;
void GameState::Render()
{
    //Only items with shadows
    mShadowEffect.Begin();
        mShadowEffect.Render(mCharacter1.renderObjects[currentRenderWorld]);
        mShadowEffect.Render(mShape[1]);
    mShadowEffect.End();

    mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter1.renderObjects[currentRenderWorld]);
        mStandardEffect.Render(mShape[1]);
        mStandardEffect.Render(mGround);
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

    currentRenderWorld = static_cast<int>(mCharas);

    if (ImGui::Combo("RenderTargetWorld", &currentRenderWorld, gCharas, static_cast<int>(std::size(gCharas))))
    {
        mCharas = (Charas)currentRenderWorld;
    }

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
    ImGui::Separator();

    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();

    ImGui::End();
}
