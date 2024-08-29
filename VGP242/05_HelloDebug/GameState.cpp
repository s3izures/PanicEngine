#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

float gRotY = 0.0f;
float gRotX = 0.0f;
const char* shapeOptions[] = { "Skybox","Skysphere","Cube","Sphere","Plane","Cylinder","Rect" };
static int currentShape = 0;
static int prevShape = 0;
MeshPC meshPC;
MeshPX meshPX;
std::filesystem::path shaderFilePX = L"../../Assets/Shaders/DoTexture.fx";
std::filesystem::path shaderFilePC = L"../../Assets/Shaders/DoTransform.fx";

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,0.0f,-50.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    //Default is Skybox
    meshPX = MeshBuilder::CreateSkyboxPX(1000);
    mConstantBuffer.Initialize(sizeof(Matrix4));

    mMeshBuffer.Initialize<MeshPX>(meshPX);

    //Vertex shader
    mVertexShader.Initialize<VertexPX>(shaderFilePX);
    mPixelShader.Initialize(shaderFilePX);
    mDiffuseTexture.Initialize("../../Assets/Images/skybox/skybox_texture.jpg");
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

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    enum ShapeOptions { Skybox, Skysphere, Cube, Sphere, Plane, Cylinder, Rect };

    if (currentShape != prevShape)
    {
        switch (currentShape)
        {
        case Skybox:
            meshPX = MeshBuilder::CreateSkyboxPX(1000);

            mMeshBuffer.Initialize<MeshPX>(meshPX);

            //Vertex shader
            mVertexShader.Initialize<VertexPX>(shaderFilePX);
            mPixelShader.Initialize(shaderFilePX);
            mDiffuseTexture.Initialize("../../Assets/Images/skybox/skybox_texture.jpg");
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        case Skysphere:
            //MeshPX mesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
            meshPX = MeshBuilder::CreateSkyspherePX(30, 30, 100.0f);

            mMeshBuffer.Initialize<MeshPX>(meshPX);

            //Vertex shader
            mVertexShader.Initialize<VertexPX>(shaderFilePX);
            mPixelShader.Initialize(shaderFilePX);
            mDiffuseTexture.Initialize("../../Assets/Images/skysphere/sky.jpg");
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        case Cube:
            meshPC = MeshBuilder::CreateCubePC(10);

            mMeshBuffer.Initialize<MeshPC>(meshPC);

            //Vertex shader
            mVertexShader.Initialize<VertexPC>(shaderFilePC);
            mPixelShader.Initialize(shaderFilePC);
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        case Sphere:
            meshPC = MeshBuilder::CreateSpherePC(10, 10, 10);

            mMeshBuffer.Initialize<MeshPC>(meshPC);

            //Vertex shader
            mVertexShader.Initialize<VertexPC>(shaderFilePC);
            mPixelShader.Initialize(shaderFilePC);
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        case Plane:
            meshPC = MeshBuilder::CreatePlanePC(5, 5, 5);

            mMeshBuffer.Initialize<MeshPC>(meshPC);

            //Vertex shader
            mVertexShader.Initialize<VertexPC>(shaderFilePC);
            mPixelShader.Initialize(shaderFilePC);
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        case Cylinder:
            meshPC = MeshBuilder::CreateCylinderPC(30, 20);

            mMeshBuffer.Initialize<MeshPC>(meshPC);

            //Vertex shader
            mVertexShader.Initialize<VertexPC>(shaderFilePC);
            mPixelShader.Initialize(shaderFilePC);
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        case Rect:
            meshPC = MeshBuilder::CreateRectPC(10, 50, 20);

            mMeshBuffer.Initialize<MeshPC>(meshPC);

            //Vertex shader
            mVertexShader.Initialize<VertexPC>(shaderFilePC);
            mPixelShader.Initialize(shaderFilePC);
            mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
            break;

        default:
            break;
        }

        prevShape = currentShape;
    }
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
    ImGui::DragFloat("rotX", &gRotX, 0.001f);
    ImGui::DragFloat("rotY", &gRotY, 0.001f);

    ImGui::ListBox("CurrentShape", &currentShape, shapeOptions, IM_ARRAYSIZE(shapeOptions), 4);

    ImGui::End();
}
