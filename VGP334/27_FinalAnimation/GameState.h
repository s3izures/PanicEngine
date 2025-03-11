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

    PanicEngine::Graphics::RenderGroup mCharacter1;
    PanicEngine::Graphics::RenderGroup mFish;
    PanicEngine::Graphics::RenderObject mGround;
    PanicEngine::Graphics::RenderObject mSky;
    PanicEngine::Graphics::Animator mCharAnim;

    PanicEngine::Graphics::Camera mCamera;
    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;

    void OnSpacePressedEvent(const PanicEngine::Event& e);
    PanicEngine::ListenerId mSpacePressedEventId = 0;
    PanicEngine::Audio::SoundId mHateId = 0;


    bool mShowSkeleton = false;
    int mAnimationIndex = -1;

    //animation
    PanicEngine::Graphics::Animation mAnimationChar1;
    PanicEngine::Graphics::Animation mAnimationFish;
    float mAnimationTime = 0.0f;
};
