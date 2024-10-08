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
    struct Vertex
    {
        PanicEngine::Math::Vector3 pos;
        PanicEngine::Color color;
    };

    using Vertices = std::vector<Vertex>;

    PanicEngine::Graphics::Camera mCamera;
    PanicEngine::Graphics::RenderObject mPlanet;

    PanicEngine::Graphics::ConstantBuffer mConstantBuffer;
    PanicEngine::Graphics::VertexShader mVertexShader;
    PanicEngine::Graphics::PixelShader mPixelShader;
    PanicEngine::Graphics::Sampler mSampler;
};
