#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;


void GameState::Initialize()
{
    MeshPX mesh = MeshBuilder::CreateSpherePX(60, 60, 1.0f);

    mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mMeshBuffer.Initialize<MeshPX>(mesh);
    mConstantBuffer.Initialize(sizeof(Matrix4));

    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mDiffuseTexture.Initialize("../../Assets/Images/planets/earth/earth.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    mSampler.Terminate();
    mDiffuseTexture.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
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
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}

void GameState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mDiffuseTexture.BindPS(0);
    mSampler.BindPS(0);

    // constant buffer
    Matrix4 matWorld = Matrix4::Identity;
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    Matrix4 matFinal = matWorld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);

    // mesh buffer
    mMeshBuffer.Render();

    Matrix4 matWorld1 = Matrix4::Identity;
    Matrix4 matView1 = mRenderTargetCamera.GetViewMatrix();
    Matrix4 matProj1 = mRenderTargetCamera.GetProjectionMatrix();
    Matrix4 matFinal1 = matWorld1 * matView1 * matProj1;
    Matrix4 wvp1 = Transpose(matFinal1);
    mConstantBuffer.Update(&wvp1);
    mConstantBuffer.BindVS(0);

    mRenderTarget.BeginRender();
    mMeshBuffer.Render();
    mRenderTarget.EndRender();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 256, 256 },
        { 0, 0 },	//uv0
        { 1, 1 },	//uv1
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });
    ImGui::End();
}