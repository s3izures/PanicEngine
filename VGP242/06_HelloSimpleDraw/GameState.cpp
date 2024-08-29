#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

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

Vector3 faceStartPos = Vector3::Zero;
Vector3 faceEndPos = Vector3::Zero;
Vector3 faceCenterPos = Vector3::Zero;
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
    SimpleDraw::AddSphere(sphereSlices, sphereRings, sphereRadius, spherePos, sphereColor);
    /*SimpleDraw::AddGroundCircle(circleSlices, circleRadius, circlePos, circleColor);
    SimpleDraw::AddTransform(Matrix4::Identity);
    SimpleDraw::AddGroundPlane(10, Colors::White);
    SimpleDraw::AddAABB(minExtents, maxExtents, aabbColor);
    SimpleDraw::AddFilledAABB(minExtents, maxExtents, aabbColor);
    SimpleDraw::AddLine(lineStartPos, lineEndPos, lineColor);
    SimpleDraw::AddFace(faceStartPos, faceCenterPos, faceEndPos, faceColor);*/
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
    /*ImGui::DragFloat3("LineStartPosition", &lineStartPos.x, 0.1f);
    ImGui::DragFloat3("LineEndPosition", &lineEndPos.x, 0.1f);
    ImGui::ColorEdit4("LineColor", &lineColor.r);

    ImGui::DragFloat3("FaceStartPosition", &faceStartPos.x, 0.1f);
    ImGui::DragFloat3("FaceCenterPosition", &faceCenterPos.x, 0.1f);
    ImGui::DragFloat3("FaceEndPosition", &faceEndPos.x, 0.1f);
    ImGui::ColorEdit4("FaceColor", &faceColor.r);

    ImGui::DragFloat3("AABBStartPosition", &minExtents.x, 0.1f);
    ImGui::DragFloat3("AABBEndPosition", &maxExtents.x, 0.1f);
    ImGui::ColorEdit4("AABBColor", &aabbColor.r);

    ImGui::DragInt("CircleSlices", &circleSlices);
    ImGui::DragFloat("CircleRadius", &circleRadius, 0.1f);
    ImGui::ColorEdit4("CircleColor", &circleColor.r);*/

    ImGui::DragInt("SphereSlices", &sphereSlices);
    ImGui::DragInt("SphereRings", &sphereRings);
    ImGui::DragFloat("SphereRadius", &sphereRadius, 0.1f);
    ImGui::ColorEdit4("SphereColor", &sphereColor.r);

    ImGui::End();
}
