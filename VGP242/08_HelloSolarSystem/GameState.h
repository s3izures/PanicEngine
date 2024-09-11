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

    std::vector<MeshPX> celestialObjects;

    PanicEngine::Graphics::Camera mCamera;
    PanicEngine::Graphics::Camera mRenderTargetCamera;
    PanicEngine::Graphics::ConstantBuffer mConstantBuffer;
    PanicEngine::Graphics::MeshBuffer mMeshBuffer;
    PanicEngine::Graphics::VertexShader mVertexShader;
    PanicEngine::Graphics::PixelShader mPixelShader;
    PanicEngine::Graphics::Texture mDiffuseTexture;
    PanicEngine::Graphics::Sampler mSampler;
    PanicEngine::Graphics::RenderTarget mRenderTarget;
};
