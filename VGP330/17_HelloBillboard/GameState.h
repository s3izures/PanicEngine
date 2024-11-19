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
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;
    
    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::BillboardEffect mBillboardEffect;
    PanicEngine::Graphics::RenderObject mGround;
    PanicEngine::Graphics::RenderObject mSprite;
};
