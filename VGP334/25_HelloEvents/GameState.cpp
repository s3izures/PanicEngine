#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;
using namespace PanicEngine::Audio;

void GameState::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";

    mCamera.SetPosition({ 0.0f,5.0f,-10.0f });
    mCamera.SetLookAt({ 0.0f,5.0f,0.0f });

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

    Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    mAnimationTime = 0.0f;
    mAnimation = AnimationBuilder()

        //bwomp
        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 0.0f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.0f)
        .AddRotationKey(Quaternion::Identity, 0.0f)

        .AddPositionKey({ 2.0f, 3.0f, 0.0f }, 0.3f)

        .AddPositionKey({ 3.0f, 4.0f, 0.0f }, 0.5f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 0.9f)

        .AddPositionKey({ 4.0f, 0.0f, 0.0f }, 1.0f)
        .AddScaleKey({ 1.5f, 0.5f, 1.5f }, 1.0f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.1f)

        //bwomp

        .AddPositionKey({ 3.0f, 3.0f, 0.0f }, 1.3f)

        .AddPositionKey({ 2.0f, 4.0f, 0.0f }, 1.5f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 1.9f)

        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 2.0f)
        .AddScaleKey({ 1.5f, 0.5f, 1.5f }, 2.0f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 2.1f)

        //bwomp

        .AddPositionKey({ -2.0f, 3.0f, 0.0f }, 2.3f)

        .AddPositionKey({ -3.0f, 4.0f, 0.0f }, 2.5f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 2.9f)

        .AddPositionKey({ -4.0f, 0.0f, 0.0f }, 3.0f)
        .AddScaleKey({ 1.5f, 0.5f, 1.5f }, 3.0f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 3.1f)

        //bwomp

        .AddPositionKey({ -3.0f, 3.0f, 0.0f }, 3.3f)

        .AddPositionKey({ -2.0f, 4.0f, 0.0f }, 3.5f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 3.9f)

        .AddPositionKey({ 0.0f, 0.0f, 0.0f }, 4.0f)
        .AddScaleKey({ 1.5f, 0.5f, 1.5f }, 4.0f)

        //star

        .AddScaleKey({ 0.5f, 1.0f, 0.5f }, 4.1f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 4.2f)
        .AddPositionKey({ 0.0f, 5.0f, 0.0f }, 4.2f)
        .AddPositionKey({ -2.0f, 4.0f, 0.0f }, 4.4f)
        .AddPositionKey({ 2.0f, 6.0f, 0.0f }, 4.6f)
        .AddPositionKey({ -2.0f, 6.0f, 0.0f }, 4.8f)
        .AddPositionKey({ 2.0f, 4.0f, 0.0f }, 5.0f)
        .AddPositionKey({ 0.0f, 7.0f, 0.0f }, 5.2f)
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 5.3f)

        .AddPositionKey({ 0.0f, 9.0f, 0.0f }, 5.4f)

        .AddPositionKey({ 0.0f, 10.0f, 0.0f }, 5.6f)

        //bwomp
        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 5.8f)

        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 6.0f)
        .AddScaleKey({ 1.0f, 0.5f, 1.0f }, 6.0f)

        .AddScaleKey({ 1.0f, 1.0f, 1.0f }, 6.2f)

        .AddPositionKey({ 0.0f, 2.0f, 0.0f }, 6.4f)

        .AddPositionKey({ 0.0f, 0.5f, 0.0f }, 6.6f)

        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 90.0f * Math::Constants::DegToRad), 6.6f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 6.8f)
        .AddRotationKey(Quaternion::CreateFromAxisAngle(Math::Vector3::XAxis, 270.0f * Math::Constants::DegToRad), 7.0f)

        .AddRotationKey(Quaternion::Identity, 8.0f)

        .AddEventKey(std::bind(&GameState::OnMoveEvent, this), 8.0f)
        .AddEventKey(std::bind(&GameState::OnAriseEvent, this), 8.1f)
        .Build();

    EventManager* em = EventManager::Get();
    mShiftPressedEventId = em->AddListener(EventType::ShiftPressed, std::bind(&GameState::OnShiftPressedEvent, this, std::placeholders::_1));
    mSpacePressedEventId = em->AddListener(EventType::SpacePressed, std::bind(&GameState::OnSpacePressedEvent, this, std::placeholders::_1));

    mGunEventId = SoundEffectManager::Get()->Load("photongun1.wav");
    mExplosionEventId = SoundEffectManager::Get()->Load("explosion.wav");
    mSwoopSFXEventId = SoundEffectManager::Get()->Load("megamanx_blast.wav");
}

void GameState::Terminate()
{
    EventManager::Get()->RemoveListener(EventType::SpacePressed, mSpacePressedEventId);
    mGround.Terminate();
    mBall.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    if (mAnimation.GetDuration() > 0.0f && playAnim)
    {
        float prevTime = mAnimationTime;
        mAnimationTime += deltaTime;
        mAnimation.PlayEvents(prevTime, mAnimationTime);
        while (mAnimationTime > mAnimation.GetDuration())
        {
            mAnimationTime -= mAnimation.GetDuration();
        }
    }

    if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
    {
        SpacePressedEvent spe;
        EventManager::Broadcast(spe);
    }

    if (InputSystem::Get()->IsKeyPressed(KeyCode::RSHIFT))
    {
        ShiftPressedEvent spev;
        EventManager::Broadcast(spev);
    }
}

void GameState::OnMoveEvent()
{
    offset.x += 0.5f;
    SoundEffectManager::Get()->Play(mExplosionEventId);
}

void GameState::OnAriseEvent()
{
    offset.y += 1.0f;
    scaleAdd.z += 3.0f;
}

void GameState::OnShiftPressedEvent(const PanicEngine::Event& e)
{
    LOG("RSHIFT PRESSED, TOGGLING ANIMATION");
    playAnim = !playAnim;
    SoundEffectManager::Get()->Play(mSwoopSFXEventId);
}

void GameState::OnSpacePressedEvent(const PanicEngine::Event& e)
{
    LOG("SPACE PRESSED, MAKING IT CHUNKIER");
    scaleAdd.x += 0.1f;
    scaleAdd.y += 0.5f;
    SoundEffectManager::Get()->Play(mGunEventId);
}

void GameState::Render()
{
    mBall.transform = mAnimation.GetTransform(mAnimationTime);
    mBall.transform.position += offset;
    mBall.transform.scale += scaleAdd;

    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mBall);
    mStandardEffect.End();
}

void GameState::UpdateCamera(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LCONTROL) ? 10.0f : 1.0f) * deltaTime;
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
    ImGui::End();
}