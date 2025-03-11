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
    void OnAriseEvent();
    void OnSpacePressedEvent(const PanicEngine::Event& e);
    void OnShiftPressedEvent(const PanicEngine::Event& e);

    PanicEngine::Graphics::Camera mCamera;

    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
    PanicEngine::Graphics::RenderObject mBall;
    PanicEngine::Graphics::RenderObject mGround;

    bool playAnim = false;
    PanicEngine::Math::Vector3 offset = { 0.0f,0.0f,0.0f };
    PanicEngine::Math::Vector3 scaleAdd = { 0.0f,0.0f,0.0f };
    PanicEngine::ListenerId mSpacePressedEventId = 0;
    PanicEngine::ListenerId mShiftPressedEventId = 0;
    PanicEngine::Audio::SoundId mGunEventId = 0;
    PanicEngine::Audio::SoundId mExplosionEventId = 0;
    PanicEngine::Audio::SoundId mSwoopSFXEventId = 0;

    //animation
    PanicEngine::Graphics::Animation mAnimationChar1;
    float mAnimationTime = 0.0f;
};