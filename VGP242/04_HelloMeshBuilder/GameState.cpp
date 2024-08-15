#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

void GameState::Initialize()
{
    MeshPC mesh = MeshBuilder::CreateSpherePC(30, 30, 1.0f);

    mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mMeshBuffer.Initialize<MeshPC>(mesh);
    mConstantBuffer.Initialize(sizeof(Matrix4));

    //Vertex shader
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
    mVertexShader.Initialize(shaderFile, VE_Position | VE_Color);
    mPixelShader.Initialize(shaderFile);
}

void GameState::Terminate()
{
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
