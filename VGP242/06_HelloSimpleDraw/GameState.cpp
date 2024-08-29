#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

const char* gDebugDrawShape[] =
{
    "None",
    "Line",
    "Sphere",
    "Circle",
    "Face",
    "Plane",
    "AABB",
    "AABBFilled",
    "Transform"
};

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,10.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

Matrix4 transMat = Matrix4::Identity;

int planeSize = 10;

Vector3 faceStartPos = Vector3::Zero;
Vector3 faceCenterPos = { 0.0f,1.0f,0.0f };
Vector3 faceEndPos = { 1.0f,0.0f,0.0f };
Color faceColor = Colors::White;

Vector3 lineStartPos = Vector3::Zero;
Vector3 lineEndPos = Vector3::Zero;
Color lineColor = Colors::White;

Vector3 minExtents = -Vector3::One;
Vector3 maxExtents = Vector3::One;
Color aabbColor = Colors::White;

Vector3 circlePos = Vector3::Zero;
int circleSlices = 100;
float circleRadius = 10;
Color circleColor = Colors::Wheat;

Vector3 spherePos = Vector3::Zero;
int sphereSlices = 30;
int sphereRings = 30;
float sphereRadius = 1;
Color sphereColor = Colors::Wheat;

void GameState::Render()
{
    switch (mDebugDrawType)
    {
    case DebugDrawType::Sphere:
        SimpleDraw::AddSphere(sphereSlices, sphereRings, sphereRadius, spherePos, sphereColor);
        break;

    case DebugDrawType::Circle:
        SimpleDraw::AddGroundCircle(circleSlices, circleRadius, circlePos, circleColor);
        break;

    case DebugDrawType::Line:
        SimpleDraw::AddLine(lineStartPos, lineEndPos, lineColor);
        break;

    case DebugDrawType::Face:
        SimpleDraw::AddFace(faceStartPos, faceCenterPos, faceEndPos, faceColor);
        break;

    case DebugDrawType::Plane:
        SimpleDraw::AddGroundPlane(planeSize, Colors::White);
        break;

    case DebugDrawType::AABB:
        SimpleDraw::AddAABB(minExtents, maxExtents, aabbColor);
        break;

    case DebugDrawType::AABBFilled:
        SimpleDraw::AddFilledAABB(minExtents, maxExtents, aabbColor);
        break;

    case DebugDrawType::Transform:
        SimpleDraw::AddTransform(transMat);
        break;

    case DebugDrawType::None:
    default:
        break;
    }

    SimpleDraw::Render(mCamera);
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
    int currentDrawType = static_cast<int>(mDebugDrawType);

    if (ImGui::Combo("DrawType", &currentDrawType, gDebugDrawShape, static_cast<int>(std::size(gDebugDrawShape))))
    {
        mDebugDrawType = (DebugDrawType)currentDrawType;
    }

    switch (mDebugDrawType)
    {
    case DebugDrawType::Sphere:
        ImGui::DragInt("SphereSlices", &sphereSlices, 1);
        ImGui::DragInt("SphereRings", &sphereRings, 1);
        ImGui::DragFloat("SphereRadius", &sphereRadius, 0.1f);
        ImGui::ColorEdit4("SphereColor", &sphereColor.r);
        break;

    case DebugDrawType::Circle:
        ImGui::DragInt("CircleSlices", &circleSlices, 1);
        ImGui::DragFloat("CircleRadius", &circleRadius, 0.1f);
        ImGui::ColorEdit4("CircleColor", &circleColor.r);
        break;

    case DebugDrawType::Line:
        ImGui::DragFloat3("LineStartPosition", &lineStartPos.x, 0.1f);
        ImGui::DragFloat3("LineEndPosition", &lineEndPos.x, 0.1f);
        ImGui::ColorEdit4("LineColor", &lineColor.r);
        break;

    case DebugDrawType::Face:
        ImGui::DragFloat3("FaceStartPosition", &faceStartPos.x, 0.1f);
        ImGui::DragFloat3("FaceCenterPosition", &faceCenterPos.x, 0.1f);
        ImGui::DragFloat3("FaceEndPosition", &faceEndPos.x, 0.1f);
        ImGui::ColorEdit4("FaceColor", &faceColor.r);
        break;

    case DebugDrawType::Plane:
        ImGui::DragInt("PlaneSize", &planeSize, 1);
        break;

    case DebugDrawType::AABB:
    case DebugDrawType::AABBFilled:
        ImGui::DragFloat3("AABBStartPosition", &minExtents.x, 0.1f);
        ImGui::DragFloat3("AABBEndPosition", &maxExtents.x, 0.1f);
        ImGui::ColorEdit4("AABBColor", &aabbColor.r);
        break;

    case DebugDrawType::Transform:
    case DebugDrawType::None:
    default:
        break;
    }
    ImGui::End();
}
