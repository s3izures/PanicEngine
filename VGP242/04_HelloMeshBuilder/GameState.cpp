#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    MeshPX mesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);

    mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mMeshBuffer.Initialize<MeshPX>(mesh);
    mConstantBuffer.Initialize(sizeof(Matrix4));

    //Vertex shader
    //std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);
    mDiffuseTexture.Initialize("../../Assets/Images/misc/basketball.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
    mSampler.Terminate();
    mDiffuseTexture.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
}

float gRotY = 0.0f;
float gRotX = 0.0f;
void GameState::Update(float deltaTime)
{
    gRotY += Math::Constants::HalfPi * deltaTime * 0.5f;
    gRotX += Math::Constants::HalfPi * deltaTime * 0.25f;
}

void GameState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mDiffuseTexture.BindPS(0);
    mSampler.BindPS(0);

    // constant buffer
    Matrix4 matWorld = Matrix4::RotationY(gRotY) * Matrix4::RotationX(gRotX);
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();
    Matrix4 matFinal = matWorld * matView * matProj;
    Matrix4 wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);
    mMeshBuffer.Render();
}
