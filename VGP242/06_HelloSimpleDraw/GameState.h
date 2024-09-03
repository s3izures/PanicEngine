#pragma once
#include <PanicEngine/Inc/PanicEngine.h>

enum class DebugDrawType
{
    None,
    Line,
    Sphere,
    Circle,
    Face,
    Plane,
    AABB,
    Transform,
    Diamond
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
    struct Vertex
    {
        PanicEngine::Math::Vector3 pos;
        PanicEngine::Color color;
    };

    using Vertices = std::vector<Vertex>;

    PanicEngine::Graphics::Camera mCamera;
    DebugDrawType mDebugDrawType = DebugDrawType::None;
};
