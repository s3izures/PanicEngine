#include "ShapeState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;


void ShapeState::CreateShape()
{
    //Front
    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Blue });

    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Green });

    //Back
    mVertices.push_back({ { -0.5f, -0.5f,0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Green });

    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });

    //Right
    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Green });

    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Green });

    //Left
    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Green });

    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Blue });

    //Top
    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { -0.5f, 0.5f, 0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });

    mVertices.push_back({ { -0.5f, 0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { 0.5f, 0.5f, -0.5f }, Colors::Green });

    //Top
    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Blue });
    mVertices.push_back({ { -0.5f, -0.5f, 0.5f }, Colors::Green });

    mVertices.push_back({ { -0.5f, -0.5f, -0.5f }, Colors::Red });
    mVertices.push_back({ { 0.5f, -0.5f, -0.5f }, Colors::Green });
    mVertices.push_back({ { 0.5f, -0.5f, 0.5f }, Colors::Blue });
}

void ShapeState::Initialize()
{
    CreateShape();

    mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mMeshBuffer.Initialize(mVertices.data(), sizeof(Vertex), mVertices.size());
    mConstantBuffer.Initialize(sizeof(Matrix4));



    //Vertex shader
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTransform.fx";
    mVertexShader.Initialize(shaderFile, VE_Position | VE_Color);

    auto device = GraphicsSystem::Get()->GetDevice();
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    
    //Pixel shader
}

void ShapeState::Terminate()
{
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertices.clear();
}

float gRotY = 0.0f;
float gRotX = 0.0f;
void ShapeState::Update(float deltaTime)
{
    gRotY += Math::Constants::HalfPi * deltaTime * 0.5f;
    gRotX += Math::Constants::HalfPi * deltaTime * 0.25f;
}

void ShapeState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetShader(mPixelShader, nullptr, 0);

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
