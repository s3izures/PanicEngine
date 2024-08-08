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

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
};
