#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

enum class Celestials
{
    None,
    Sun,
    Mercury,
    Venus,
    Earth,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune,
    Pluto
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
    PanicEngine::Graphics::Camera mRenderTargetCamera;
    PanicEngine::Graphics::ConstantBuffer mConstantBuffer[11];
    PanicEngine::Graphics::MeshBuffer mMeshBuffer[11];
    PanicEngine::Graphics::VertexShader mVertexShader[11];
    PanicEngine::Graphics::PixelShader mPixelShader[11];
    PanicEngine::Graphics::Texture mDiffuseTexture[11];
    PanicEngine::Graphics::Sampler mSampler[11];
    PanicEngine::Graphics::RenderTarget mRenderTarget;

    Celestials mCelestials = Celestials::None;
};
