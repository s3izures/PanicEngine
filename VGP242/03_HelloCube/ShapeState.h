#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

class ShapeState : public PanicEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;

protected:
    virtual void CreateShape();
    struct Vertex
    {
        PanicEngine::Math::Vector3 pos;
        PanicEngine::Color color;
    };

    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    PanicEngine::Graphics::Camera mCamera;
    PanicEngine::Graphics::ConstantBuffer mConstantBuffer;
    PanicEngine::Graphics::MeshBuffer mMeshBuffer;
    PanicEngine::Graphics::VertexShader mVertexShader;
    PanicEngine::Graphics::PixelShader mPixelShader;
};
