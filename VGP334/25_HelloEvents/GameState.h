#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

class GameState : public PanicEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI();

protected:
    void UpdateCamera(float deltaTime);
    void OnMoveEvent();
    void OnSpacePressedEvent(const PanicEngine::Event& e);

    PanicEngine::Graphics::Camera mCamera;

    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
    PanicEngine::Graphics::RenderObject mBall;
    PanicEngine::Graphics::RenderObject mGround;

    PanicEngine::Math::Vector3 offset = { 0.0f,0.0f,0.0f };
    PanicEngine::ListenerId mSpacePressedEventId = 0;

    //animation
    PanicEngine::Graphics::Animation mAnimation;
    float mAnimationTime = 0.0f;
};