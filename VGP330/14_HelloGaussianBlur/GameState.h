#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

enum class Charas
{
    Prisoner,
    Amy
};

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
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
    
    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::GaussianBlurEffect mGaussianBlurEffect;
    PanicEngine::Graphics::PostProcessEffect mPostProcessEffect;

    PanicEngine::Graphics::RenderGroup mCharacters;
    PanicEngine::Graphics::RenderObject mGround;
    PanicEngine::Graphics::RenderObject mScreenQuad;

    PanicEngine::Graphics::RenderTarget mRenderTarget;
    PanicEngine::Graphics::RenderTarget mBloomRenderTarget;

    Charas mCharas = Charas::Prisoner;
};
