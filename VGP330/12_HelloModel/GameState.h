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
    
    PanicEngine::Graphics::RenderGroup mCharacter1;
    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::StandardEffect mRenderTargetStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
    PanicEngine::Graphics::RenderTarget mRenderTarget;

    Charas mCharas = Charas::Prisoner;
    Charas mCharasBox = Charas::Amy;
};
