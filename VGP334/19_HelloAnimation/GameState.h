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

    PanicEngine::Graphics::Camera mCamera;

    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
    PanicEngine::Graphics::RenderObject mBall;
    PanicEngine::Graphics::RenderObject mGround;

    //animation
    PanicEngine::Graphics::Animation mAnimationChar1;
    float mAnimationTime = 0.0f;
};