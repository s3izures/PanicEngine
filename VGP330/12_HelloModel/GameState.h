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
    
    PanicEngine::Graphics::RenderGroup mCharacters;
    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
};
