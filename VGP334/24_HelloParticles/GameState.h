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

    PanicEngine::Graphics::RenderObject mBall;
    PanicEngine::Physics::CollisionShape mBallShape;
    PanicEngine::Physics::RigidBody mBallRB;

    PanicEngine::Graphics::StandardEffect mStandardEffect;
    PanicEngine::Graphics::DirectionalLight mDirectionalLight;

    PanicEngine::Graphics::RenderObject mGround;
    PanicEngine::Physics::CollisionShape mGroundShape;
    PanicEngine::Physics::RigidBody mGroundRB;

    struct BoxData
    {
        PanicEngine::Graphics::RenderObject box;
        PanicEngine::Physics::CollisionShape boxShape;
        PanicEngine::Physics::RigidBody boxRB;
    };
    using Boxes = std::vector<BoxData>;
    Boxes mBoxes;

};