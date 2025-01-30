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

    PanicEngine::Graphics::RenderGroup mCharacters;
    PanicEngine::Graphics::RenderObject mGround;
    PanicEngine::Graphics::Animator mCharAnim;

    PanicEngine::Graphics::Camera mCamera;
    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;


    bool mShowSkeleton = false;
    int mAnimationIndex = -1;
};
