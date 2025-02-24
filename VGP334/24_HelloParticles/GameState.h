#pragma once

#include <PanicEngine/Inc/PanicEngine.h>


class GameState : public PanicEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void DebugUI() override;
	void Update(float deltaTime);

protected:
	void UpdateCamera(float deltaTime);
	PanicEngine::Graphics::Camera mCamera;
	PanicEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	PanicEngine::Physics::ParticleSystem mParticleSystem;
};