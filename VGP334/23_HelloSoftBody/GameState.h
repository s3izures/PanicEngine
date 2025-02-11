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

    // cloth info
    PanicEngine::Graphics::RenderObject mCloth;
    PanicEngine::Graphics::Mesh mClothMesh;
    PanicEngine::Physics::SoftBody mClothSB;

};