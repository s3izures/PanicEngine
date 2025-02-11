#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;
using namespace PanicEngine::Physics;

void GameState::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";

    mCamera.SetPosition({ 0.0f,1.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f,0.9f,1.0f };

    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Mesh ball = MeshBuilder::CreateSphere(60, 60, 0.5f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureCache::Get()->LoadTexture("misc/basketball.jpg");

    mBallShape.InitializeSphere(0.5f);
    mBallRB.Initialize(mBall.transform, mBallShape, 1.0f);

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");
    mGroundShape.InitializeHull({ 5.0f, 0.5f, 5.0f }, { 0.0f, -0.5f, 0.0f });
    mGroundRB.Initialize(mGround.transform, mGroundShape);

    Mesh boxShape = MeshBuilder::CreateCube(1.0f);
    TextureId boxTex = TextureCache::Get()->LoadTexture("sprites/yellow.jpg");

    int rows = 10;
    int cols = 10;

    mClothMesh = MeshBuilder::CreateGroundPlane(rows, cols, 1.0f);
    for (Vertex& v : mClothMesh.vertices)
    {
        v.position.y = 10.0f;
    }
    uint32_t lastVertex = mClothMesh.vertices.size() - 1;
    uint32_t lastVertexOS = lastVertex - cols;
    mClothSB.Initialize(mClothMesh, 1.0f, { lastVertex, lastVertexOS });
    mCloth.meshBuffer.Initialize(nullptr, sizeof(Vertex), mClothMesh.vertices.size(), mClothMesh.indices.data(), mClothMesh.indices.size());
    TextureId clothTexture = TextureCache::Get()->LoadTexture("sprites/splashscreen.bmp");
}

void GameState::Terminate()
{
    mCloth.Terminate();
    mClothSB.Terminate();
    mGroundRB.Terminate();
    mGroundShape.Terminate();
    mGround.Terminate();
    mBallRB.Terminate();
    mBallShape.Terminate();
    mBall.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
    {
        mBallRB.SetVelocity({ 0.0f, 5.0f, 0.2f });

    }

    if (InputSystem::Get()->IsMousePressed(MouseButton::LBUTTON))
    {
        Math::Vector3 spawnPos = mCamera.GetPosition() + mCamera.GetDirection() * 0.5f;
        mBallRB.SetPosition(spawnPos);
        mBallRB.SetVelocity(mCamera.GetDirection() * 20.0f);
    }
}

void GameState::Render()
{
    mCloth.meshBuffer.Update(mClothMesh.vertices.data(), mClothMesh.vertices.size());
    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mCloth);
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

    mStandardEffect.DebugUI();
    PhysicsWorld::Get()->DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCamera);
}