#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;
using namespace PanicEngine::Physics;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 2.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mParticleSystemEffect.Initialize();
	mParticleSystemEffect.SetCamera(mCamera);

	ParticleSystemInfo info;
	info.textureId = TextureCache::Get()->LoadTexture("sprites/explosion.png");
	int maxParticles = 100;
	info.particlesPerEmit = { 1, 4 };
	info.delay = 1.0f;
	info.lifeTime = FLT_MAX;
	info.timeBetweenEmit = { 0.2f, 0.4f };
	info.spawnAngle = { -30.0f, 30.0f };
	info.spawnSpeed = { 1.0f, 3.0f };
	info.spawnLifeTime = { 0.5f, 2.0f };
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnPosition = Math::Vector3::Zero;
	info.startScale = { Math::Vector3::One, Math::Vector3::One };
	info.endScale = { Math::Vector3::One, Math::Vector3::One };
	info.startColor = { Colors::White, Colors::White };
	info.endColor = { Colors::White, Colors::White };
	mParticleSystem.Initialize(info);
}

void GameState::Terminate()
{
	mParticleSystem.Terminate();
	mParticleSystemEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	mParticleSystem.Update(deltaTime);
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
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

bool checkBox = true;

void GameState::Render()
{
	mParticleSystemEffect.Begin();
		mParticleSystem.Render(mParticleSystemEffect);
	mParticleSystemEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mParticleSystem.DebugUI();
	mParticleSystemEffect.DebugUI();
	PhysicsWorld::Get()->DebugUI();
	ImGui::End();

	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);
}