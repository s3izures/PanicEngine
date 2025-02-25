#include "Precompiled.h"
#include "ParticleSystem.h"

using namespace PanicEngine;
using namespace PanicEngine::Physics;
using namespace PanicEngine::Graphics;

void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailableParticleIndex = 0;
	mNextSpawnTime = info.delay;
	mLifeTime = info.lifeTime;

	InitializeParticles(info.maxParticles);
}

void ParticleSystem::InitializeParticles(uint32_t count)
{
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticles[i] = std::make_unique<Particle>();
		mParticles[i]->Initialize();
	}
}

void ParticleSystem::Terminate()
{
	for (auto& particle : mParticles)
	{
		particle->Terminate();
		particle.reset();
	}
	mParticles.clear();
}

void ParticleSystem::Update(float deltaTime)
{
	if (IsActive())
	{
		mLifeTime -= deltaTime;
		mNextSpawnTime -= deltaTime;

		if (mNextSpawnTime <= 0.0f && mLifeTime > 0.0f)
		{
			SpawnParticles();
		}
		for (auto& particle : mParticles)
		{
			particle->Update(deltaTime);
		}
	}
}

bool ParticleSystem::IsActive()
{
	if (mLifeTime > 0.0f)
	{
		return true;
	}
	for (auto& particle : mParticles)
	{
		if (particle->IsActive())
		{
			return true;
		}
	}
	return false;
}

void ParticleSystem::DebugUI()
{
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::DragFloat3("Position", &mInfo.spawnPosition.x);
		if (ImGui::DragFloat3("Direction", &mInfo.spawnDirection.x))
		{
			mInfo.spawnDirection = Math::Normalize(mInfo.spawnDirection);
		}
		ImGui::DragIntRange2("PerEmit", &mInfo.particlesPerEmit.min, &mInfo.particlesPerEmit.max);
		ImGui::DragFloatRange2("TimeBetweenEmit", &mInfo.timeBetweenEmit.min, &mInfo.timeBetweenEmit.max);
		ImGui::DragFloatRange2("SpawnAngle", &mInfo.spawnAngle.min, &mInfo.spawnAngle.max);
		ImGui::DragFloatRange2("SpawnSpeed", &mInfo.spawnSpeed.min, &mInfo.spawnSpeed.max);
		ImGui::DragFloatRange2("SpawnLifeTime", &mInfo.spawnLifeTime.min, &mInfo.spawnLifeTime.max);
		ImGui::DragFloat3("StartScaleMin", &mInfo.startScale.min.x);
		ImGui::DragFloat3("StartScaleMax", &mInfo.startScale.max.x);
		ImGui::DragFloat3("EndScaleMin", &mInfo.endScale.min.x);
		ImGui::DragFloat3("EndScaleMax", &mInfo.endScale.max.x);
		ImGui::ColorEdit4("StartColorMin", &mInfo.startColor.min.r);
		ImGui::ColorEdit4("StartColorMax", &mInfo.startColor.max.r);
		ImGui::ColorEdit4("EndColorMin", &mInfo.endColor.min.r);
		ImGui::ColorEdit4("EndColorMax", &mInfo.endColor.max.r);
	}
}

void ParticleSystem::SetPosition(const Math::Vector3& position)
{
	mInfo.spawnPosition = position;
}

void ParticleSystem::SpawnParticles()
{
	int numParticles = mInfo.particlesPerEmit.GetRandomInc();
	for (int i = 0; i < numParticles; ++i)
	{
		SpawnSingleParticle();
	}

	mNextSpawnTime = mInfo.timeBetweenEmit.GetRandom();
}

void ParticleSystem::Render(Graphics::ParticleSystemEffect& effect)
{
	if (!IsActive())
	{
		return;
	}
	effect.SetTextureID(mInfo.textureId);
	for (auto& particle : mParticles)
	{
		if (particle->IsActive())
		{
			effect.Render(particle->GetTransform(), particle->GetColor());
		}
	}
}

void ParticleSystem::SpawnSingleParticle()
{
	Particle* particle = mParticles[mNextAvailableParticleIndex].get();
    mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mInfo.maxParticles;

    const bool isUp = (abs(Math::Dot(mInfo.spawnDirection, Math::Vector3::YAxis))) > 0.999f;
    Math::Vector3 r = (isUp) ? Math::Vector3::XAxis : Math::Normalize(Math::Cross(Math::Vector3::YAxis, mInfo.spawnDirection));
    Math::Vector3 u = (isUp) ? Math::Vector3::ZAxis : Math::Normalize(Math::Cross(mInfo.spawnDirection, r));
    
    float rotAngle = mInfo.spawnAngle.GetRandom() * Math::Constants::DegToRad;
    Math::Matrix4 matRotRight = Math::Matrix4::RotationAxis(r, rotAngle);
    
    rotAngle = mInfo.spawnAngle.GetRandom() * Math::Constants::DegToRad;
    Math::Matrix4 matRotUp = Math::Matrix4::RotationAxis(u, rotAngle);
    Math::Matrix4 matRotation = matRotRight * matRotUp;
    Math::Vector3 spawnDirection = Math::TransformCoord(mInfo.spawnDirection, matRotation);

    const float speed = mInfo.spawnSpeed.GetRandom();
    ParticleInfo info;
    info.lifeTime = mInfo.spawnLifeTime.GetRandom();
    info.startColor = mInfo.startColor.GetRandom();
    info.endColor = mInfo.endColor.GetRandom();
    info.startScale = mInfo.startScale.GetRandom();
    info.endScale = mInfo.endScale.GetRandom();

    particle->Activate(info, mInfo.spawnPosition, spawnDirection * speed);
}