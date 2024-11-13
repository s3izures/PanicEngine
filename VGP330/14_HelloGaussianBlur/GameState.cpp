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

    mGaussianBlurEffect.Initialize();
    mGaussianBlurEffect.SetSourceTexture(mBloomRenderTarget);

    mPostProcessEffect.Initialize(shaderFilePP);
    mPostProcessEffect.SetTexture(&mRenderTarget);
    mPostProcessEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(), 1);
    mPostProcessEffect.SetMode(PostProcessEffect::Mode::Combine2);

    mCharacters.Initialize(L"../../Assets/Models/Prisoner/Prisoner.model");
    mCharacters.Initialize(L"../../Assets/Models/Amy/Amy.model");

    MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
    mScreenQuad.meshBuffer.Initialize(screenQuad);

    Mesh groundMesh = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenX = gs->GetBackBufferWidth();
    const uint32_t screenY = gs->GetBackBufferHeight();
    mRenderTarget.Initialize(screenX, screenY, Texture::Format::RGBA_U8);
    mBloomRenderTarget.Initialize(screenX, screenY, Texture::Format::RGBA_U8);
}

void GameState::Terminate()
{
    mBloomRenderTarget.Terminate();
    mRenderTarget.Terminate();
    mGround.Terminate();
    mScreenQuad.Terminate();
    mCharacters.Terminate();
    mPostProcessEffect.Terminate();
    mGaussianBlurEffect.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

int currentRenderWorld = 0;
void GameState::Render()
{
    mCamera.SetAspectRatio(0.0f);

    mRenderTarget.BeginRender();
        mStandardEffect.Begin();
            mStandardEffect.Render(mCharacters, currentRenderWorld);
            mStandardEffect.Render(mGround);
        mStandardEffect.End();
    mRenderTarget.EndRender();

    mCamera.SetAspectRatio(1.0f);

    mBloomRenderTarget.BeginRender();
        mStandardEffect.Begin();
            mStandardEffect.Render(mCharacters, currentRenderWorld);
        mStandardEffect.End();
    mBloomRenderTarget.EndRender();

    mGaussianBlurEffect.Begin();
        mGaussianBlurEffect.Render(mScreenQuad);
    mGaussianBlurEffect.End();

    mPostProcessEffect.Begin();
        mPostProcessEffect.Render(mScreenQuad);
    mPostProcessEffect.End();
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
    ImGui::Text("RenderTarget");
    ImGui::Image(
        mRenderTarget.GetRawData(),
    { 200, 200 },
    { 0,0 },
    { 1,1 },
    { 1,1,1,1 },
    { 1,1,1,1 });


    mStandardEffect.DebugUI();
    mGaussianBlurEffect.DebugUI();
    ImGui::End();
}
