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
    PanicEngine::Graphics::ShadowEffect mShadowEffect;

    PanicEngine::Graphics::RenderGroup mCharacter1;
    PanicEngine::Graphics::RenderObject mShape[2];
    PanicEngine::Graphics::RenderObject mGround;

    Charas mCharas = Charas::Prisoner;
};
