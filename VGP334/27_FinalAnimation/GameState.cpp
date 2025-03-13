#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;
using namespace PanicEngine::Physics;
using namespace PanicEngine::Audio;

PanicEngine::Math::Vector3 Char1Pos;
bool fishSpin = false;
int followChar = 0;
bool renderChar1 = true;
bool renderChar2 = true;
bool activeParticles = false;

void GameState::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";

    mCamera.SetPosition({ 0.000000, 5.823244, -29.886192 });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f,0.9f,1.0f };

    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    //Fish
    mFish.Initialize(L"../../Assets/Models/SmokedFish/SmokedFish.model");

    //Amy
    mCharacter1.Initialize(L"../../Assets/Models/Amy/Amy.model", &mCharAnim);
    ModelCache::Get()->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Amy/Defeated.animset"); //1
    ModelCache::Get()->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Amy/Reaction.animset"); //2
    ModelCache::Get()->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Amy/JumpingDown.animset"); //3
    ModelCache::Get()->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Amy/HipHopDancing.animset"); //4
    ModelCache::Get()->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Amy/TwistDance.animset"); //5
    mCharAnim.Initialize(mCharacter1.modelId);
    
    //Prisoner
    mCharacter2.Initialize(L"../../Assets/Models/Prisoner/Prisoner.model", &mCharAnim2);
    ModelCache::Get()->AddAnimation(mCharacter2.modelId, L"../../Assets/Models/Prisoner/RobotDance.animset"); //1
    mCharAnim2.Initialize(mCharacter2.modelId);

    //Particles
    mParticleSystemEffect.Initialize();
    mParticleSystemEffect.SetCamera(mCamera);

    ParticleSystemInfo info;
    info.textureId = TextureCache::Get()->LoadTexture("sprites/explosion.png");
    int maxParticles = 100;
    info.particlesPerEmit = { 1, 4 };
    info.delay = 1.0f;
    info.lifeTime = FLT_MAX;
    info.timeBetweenEmit = { 0.1f, 0.2f };
    info.spawnAngle = { -30.0f, 30.0f };
    info.spawnSpeed = { 6.0f, 7.0f };
    info.spawnLifeTime = { 0.5f, 2.0f };
    info.spawnDirection = Math::Vector3::YAxis;
    info.startScale = { Math::Vector3::Zero, Math::Vector3::Zero };
    info.endScale = { Math::Vector3::One, Math::Vector3::One };
    info.startColor = { Colors::White, Colors::White };
    info.endColor = { Colors::White, Colors::White };
    mParticleSystem.Initialize(info);

    Mesh sphere = MeshBuilder::CreateSkysphere(100, 100, 1000.0f);
    mSky.meshBuffer.Initialize(sphere);
    mSky.diffuseMapId = TextureCache::Get()->LoadTexture("skysphere/sunrise.jpg");

    Mesh ground = MeshBuilder::CreateGroundPlane(500, 500, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseMapId = TextureCache::Get()->LoadTexture("skysphere/sky.jpg");

    EventManager* em = EventManager::Get();
    mSpacePressedEventId = em->AddListener(EventType::SpacePressed, std::bind(&GameState::OnSpacePressedEvent, this, std::placeholders::_1));

    mHateId = SoundEffectManager::Get()->Load("HATE.wav");

    mAnimationTime = 0.0f;
    mAnimationChar1 = AnimationBuilder()
        .AddScaleKey({ 1.0f,1.0f,1.0f }, 0.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, -85.0f * Math::Constants::DegToRad), 0.1f)
        .AddEventKey([&]()
            {
                renderChar1 = true;
                mCamera.SetPosition({ 0.000000, 5.823244, -29.886192 });
                mCharAnim.PlayAnimation(3, false);
                followChar = 1;
            }, 0.1f) //RESET
        .AddEventKey([&]() {activeParticles = false;}, 0.1f)
        .AddEventKey([&]()
            {
                followChar = 0;
            }, 9.0f)
        .AddEventKey([&]() {activeParticles = true;}, 10.0f)
        .AddPositionKey({ -50.0f,0.0f,0.0f }, 10.0f)
        .AddEventKey([&]()
            {
                mCharAnim.PlayAnimation(2, true);
            }, 10.5f)
        .AddEventKey([&]()
            {
                mCamera.SetPosition({ 1.983818f, 1.732380f, -2.305765f });
                mCharAnim.PlayAnimation(5, false);
            }, 11.0f)
        .AddPositionKey({ 0.0f, 0.0f,0.0f }, 12.0f)
        .AddEventKey([&]() {activeParticles = false;}, 13.0f)
        .AddEventKey([&]()
            {
                mCharAnim.PlayAnimation(4, true);
            }, 13.0f)
        .AddPositionKey({ 0.0f,-0.1f,0.0f }, 20.0f)
        .AddEventKey([&]()
            {
                mCharAnim.PlayAnimation(1, false);
            }, 20.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, -85.0f * Math::Constants::DegToRad), 20.1f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::ZAxis, 100000000.0f * Math::Constants::DegToRad), 25.0f)
        .AddEventKey([&]()
            {
                mCharAnim.PlayAnimation(4, true);
            }, 25.0f)
        .AddScaleKey({ 1.0f,1.0f,1.0f }, 30.0f)
        .AddPositionKey({ 0.0f,5.0f,0.0f }, 35.0f)
        .AddScaleKey({ 50.0f,1.0f,0.5f }, 40.0f)
        .AddScaleKey({ 0.5f,0.5f,0.5f }, 43.5f)
        .AddPositionKey({ 0.0f,10.0f,0.0f }, 45.0f)
        .AddEventKey([&]()
            {
                mCamera.SetPosition({ 0.000000, 5.823244, -29.886192 });
                followChar = 1;
                renderChar1 = false;
            }, 45.0f)
        .AddEventKey([&]()
            {
                mCamera.SetPosition({ 0.000000, 5.823244, -29.886192 });
                mCharAnim.PlayAnimation(-1, false);
            }, 57.0f) //RESET
        .Build();

    mAnimationFish = AnimationBuilder()
        .AddPositionKey({ 0.0f, 0.0f,0.0f }, 0.0f)
        .AddEventKey([&]() {SoundEffectManager::Get()->Play(mHateId);}, 0.1f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 0.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::ZAxis, 180.0f * Math::Constants::DegToRad), 1.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 2.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::YAxis, 180.0f * Math::Constants::DegToRad), 3.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 0.0f * Math::Constants::DegToRad), 4.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 5.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 360.0f * Math::Constants::DegToRad), 6.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::ZAxis, 180.0f * Math::Constants::DegToRad), 7.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 0.0f * Math::Constants::DegToRad), 8.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 180.0f * Math::Constants::DegToRad), 9.0f)
        .AddPositionKey({ 0.0f, 5.0f,0.0f }, 9.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::YAxis, 180.0f * Math::Constants::DegToRad), 10.0f)
        .AddPositionKey({ 0.0f, 10.0f,0.0f }, 20.0f)
        .AddPositionKey({ 0.0f, 10.0f,0.0f }, 45.0f)
        .AddRotationKey(PanicEngine::Math::Quaternion::CreateFromAxisAngle(Vector3::XAxis, 0.0f * Math::Constants::DegToRad), 45.0f)
        .AddPositionKey({-10.0f, 3.0f, 0.0f}, 50.0f)
        .AddPositionKey({0.0f, 3.0f, -50.0f}, 57.0f)
        .Build();

    mAnimationChar2 = AnimationBuilder()
        .AddEventKey([&]() {mCharAnim2.PlayAnimation(1, true);}, 0.1f)
        .AddEventKey([&]() {mCamera.SetPosition({ -2.0f, 3.732380f, -2.305765f }); followChar = 2;}, 54.0f)
        .Build();

}

void GameState::Terminate()
{
    EventManager::Get()->RemoveListener(EventType::SpacePressed, mSpacePressedEventId);
    mGround.Terminate();
    mSky.Terminate();
    mParticleSystem.Terminate();
    mParticleSystemEffect.Terminate();
    mCharacter2.Terminate();
    mCharacter1.Terminate();
    mFish.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    mParticleSystem.Update(deltaTime);
    mParticleSystem.SetPosition(mCharacter1.transform.position);

    //UpdateCamera(deltaTime);
    mCharAnim.Update(deltaTime);
    mCharAnim2.Update(deltaTime);

    if (mAnimationChar1.GetDuration() > 0.0f && mAnimationFish.GetDuration() > 0.0f && mAnimationChar2.GetDuration() > 0.0f)
    {
        float prevTime = mAnimationTime;
        mAnimationTime += deltaTime;
        mAnimationChar1.PlayEvents(prevTime, mAnimationTime);
        mAnimationChar2.PlayEvents(prevTime, mAnimationTime);
        mAnimationFish.PlayEvents(prevTime, mAnimationTime);
        while (mAnimationTime > mAnimationChar1.GetDuration() && mAnimationTime > mAnimationFish.GetDuration())
        {
            mAnimationTime -= mAnimationChar1.GetDuration();
        }
    }

    if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
    {
        SpacePressedEvent spe;
        EventManager::Broadcast(spe);
    }

    if (followChar == 0)
    {
        mCamera.SetLookAt(mCharacter1.transform.position);
    }
    else if (followChar == 1)
    {
        mCamera.SetLookAt(mFish.transform.position);
    }
    else if (followChar == 2)
    {
        mCamera.SetLookAt(mCharacter2.transform.position);
    }
}

void GameState::OnSpacePressedEvent(const PanicEngine::Event& e)
{
    LOG("Pos: %f, %f, %f", mCamera.GetPosition().x, mCamera.GetPosition().y, mCamera.GetPosition().z);
}

void GameState::Render()
{
    mFish.transform = mAnimationFish.GetTransform(mAnimationTime);
    mCharacter1.transform = mAnimationChar1.GetTransform(mAnimationTime);

    mStandardEffect.Begin();
        if (mShowSkeleton)
        {
            AnimationUtil::BoneTransforms boneTransforms;
            AnimationUtil::ComputeBoneTransforms(mCharacter1.modelId, boneTransforms, &mCharAnim);
            AnimationUtil::DrawSkeleton(mCharacter1.modelId, boneTransforms);
            AnimationUtil::ComputeBoneTransforms(mCharacter2.modelId, boneTransforms, &mCharAnim);
            AnimationUtil::DrawSkeleton(mCharacter2.modelId, boneTransforms);
        }
        else
        {
            mStandardEffect.Render(mFish);
            if (renderChar1)
            {
                mStandardEffect.Render(mCharacter1);
            }
            if (renderChar2)
            {
                mStandardEffect.Render(mCharacter2);
            }
        }
        mStandardEffect.Render(mGround);
        mStandardEffect.Render(mSky);
    mStandardEffect.End();

    mParticleSystemEffect.Begin();
        if (activeParticles)
        {
            mParticleSystem.Render(mParticleSystemEffect);
        }
    mParticleSystemEffect.End();
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

    mParticleSystem.DebugUI();
    mParticleSystemEffect.DebugUI();
    PhysicsWorld::Get()->DebugUI();

    //mStandardEffect.DebugUI();

    ImGui::Separator();

    ImGui::Text(std::to_string(mAnimationTime).c_str());

    //ImGui::Checkbox("ShowSkeleton", &mShowSkeleton);
    /*int maxAnimations = mCharAnim.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex", &mAnimationIndex, 1, -1, maxAnimations - 1))
    {
        mCharAnim.PlayAnimation(mAnimationIndex, true);
    }*/
    //SimpleDraw::Render(mCamera);
    ImGui::End();
}
