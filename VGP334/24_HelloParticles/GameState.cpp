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

    float yOffset = 4;
    float xOffset = 0.0f;
    int rowCount = 1;
    int boxIndex = 0;
    mBoxes.resize(10);
    while (boxIndex < 10)
    {
        xOffset = -((static_cast<float>(rowCount) - 1.0f) * 0.5f);
        for (int r = 0; r < rowCount; ++r)
        {
            BoxData& newBox = mBoxes[boxIndex];
            newBox.box.meshBuffer.Initialize(boxShape);
            newBox.box.diffuseMapId = boxTex;
            newBox.box.transform.position.y = yOffset;
            newBox.box.transform.position.x = xOffset;
            newBox.box.transform.position.z = 4.0f;
            newBox.boxShape.InitializeBox({ 0.5f,0.5f,0.5f });
            
            xOffset += 1.0f;
            ++boxIndex;
        }
        yOffset -= 1.0f;
        rowCount += 1;
    }

    for (BoxData& box : mBoxes)
    {
        box.boxRB.Initialize(box.box.transform, box.boxShape, 1.0f);
    }
}

void GameState::Terminate()
{
    for (BoxData& box : mBoxes)
    {
        box.boxRB.Terminate();
        box.boxShape.Terminate();
    }

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
    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
        for (BoxData& box : mBoxes)
        {
            mStandardEffect.Render(box.box);
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

    mStandardEffect.DebugUI();
    PhysicsWorld::Get()->DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCamera);
}