#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

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

    mCharacters.Initialize(L"../../Assets/Models/Amy/Amy.model", &mCharAnim);
    ModelCache::Get()->AddAnimation(mCharacters.modelId, L"../../Assets/Models/Amy/Defeated.animset");
    ModelCache::Get()->AddAnimation(mCharacters.modelId, L"../../Assets/Models/Amy/Reaction.animset");
    ModelCache::Get()->AddAnimation(mCharacters.modelId, L"../../Assets/Models/Amy/Praying.animset");
    ModelCache::Get()->AddAnimation(mCharacters.modelId, L"../../Assets/Models/Amy/JumpPushUp.animset");
    ModelCache::Get()->AddAnimation(mCharacters.modelId, L"../../Assets/Models/Amy/TwistDance.animset");
    mCharAnim.Initialize(mCharacters.modelId);

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");
}

void GameState::Terminate()
{
    mGround.Terminate();
    mCharacters.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    mCharAnim.Update(deltaTime);
}

void GameState::Render()
{
    mStandardEffect.Begin();
        if (mShowSkeleton)
        {
            AnimationUtil::BoneTransforms boneTransforms;
            AnimationUtil::ComputeBoneTransforms(mCharacters.modelId, boneTransforms, &mCharAnim);
            AnimationUtil::DrawSkeleton(mCharacters.modelId, boneTransforms);
        }
        else
        {
            mStandardEffect.Render(mCharacters);
        }
        mStandardEffect.Render(mGround);
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

    ImGui::Separator();

    ImGui::Checkbox("ShowSkeleton", &mShowSkeleton);
    int maxAnimations = mCharAnim.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex", &mAnimationIndex, 1, -1, maxAnimations - 1))
    {
        mCharAnim.PlayAnimation(mAnimationIndex, true);
    }
    SimpleDraw::Render(mCamera);

    ImGui::End();
}
