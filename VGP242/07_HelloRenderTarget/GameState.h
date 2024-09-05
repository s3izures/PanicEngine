#pragma once

#include <PanicEngine/Inc/PanicEngine.h>

class GameState : public PanicEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Render() override;
    void DebugUI() override;

    void Update(float deltaTime);
protected:
    void UpdateCamera(float deltaTime);

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
