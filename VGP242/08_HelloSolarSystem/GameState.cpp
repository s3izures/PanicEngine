#include "GameState.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

const char* gCelestials[] =
{
    "None",
    "Sun",
    "Mercury",
    "Venus",
    "Earth",
    "Mars",
    "Jupiter",
    "Saturn",
    "Uranus",
    "Neptune",
    "Pluto"
};

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 0.0f, -300.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, -300.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    MeshPX space = MeshBuilder::CreateSkyspherePX(100, 100, 500.0f);
    MeshPX sun = MeshBuilder::CreateSpherePX(100, 100, 109.0f);

    MeshPX mercury = MeshBuilder::CreateSpherePX(100, 100, 0.35f);
    MeshPX venus = MeshBuilder::CreateSpherePX(100, 100, 0.8f);
    MeshPX earth = MeshBuilder::CreateSpherePX(100, 100, 1.0f);
    MeshPX mars = MeshBuilder::CreateSpherePX(100, 100, 2.0f);
    MeshPX jupiter = MeshBuilder::CreateSpherePX(100, 100, 11.0f);
    MeshPX saturn = MeshBuilder::CreateSpherePX(100, 100, 10.0f);
    MeshPX uranus = MeshBuilder::CreateSpherePX(100, 100, 4.0f);
    MeshPX neptune = MeshBuilder::CreateSpherePX(100, 100, 3.0f);
    MeshPX pluto = MeshBuilder::CreateSpherePX(100, 100, 0.2f);

    mMeshBuffer[0].Initialize<MeshPX>(space);
    mMeshBuffer[1].Initialize<MeshPX>(sun);
    mMeshBuffer[2].Initialize<MeshPX>(mercury);
    mMeshBuffer[3].Initialize<MeshPX>(venus);
    mMeshBuffer[4].Initialize<MeshPX>(earth);
    mMeshBuffer[5].Initialize<MeshPX>(mars);
    mMeshBuffer[6].Initialize<MeshPX>(jupiter);
    mMeshBuffer[7].Initialize<MeshPX>(saturn);
    mMeshBuffer[8].Initialize<MeshPX>(uranus);
    mMeshBuffer[9].Initialize<MeshPX>(neptune);
    mMeshBuffer[10].Initialize<MeshPX>(pluto);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    for (int i = 0; i < 11; i++)
    {
        mConstantBuffer[i].Initialize(sizeof(Matrix4));

        mVertexShader[i].Initialize<VertexPX>(shaderFile);
        mPixelShader[i].Initialize(shaderFile);
    }

    mDiffuseTexture[0].Initialize("../../Assets/Images/skysphere/space.jpg");
    mDiffuseTexture[1].Initialize("../../Assets/Images/planets/sun.jpg");
    mDiffuseTexture[2].Initialize("../../Assets/Images/planets/mercury.jpg");
    mDiffuseTexture[3].Initialize("../../Assets/Images/planets/venus.jpg");
    mDiffuseTexture[4].Initialize("../../Assets/Images/planets/earth/earth.jpg");
    mDiffuseTexture[5].Initialize("../../Assets/Images/planets/mars.jpg");
    mDiffuseTexture[6].Initialize("../../Assets/Images/planets/jupiter.jpg");
    mDiffuseTexture[7].Initialize("../../Assets/Images/planets/saturn.jpg");
    mDiffuseTexture[8].Initialize("../../Assets/Images/planets/uranus.jpg");
    mDiffuseTexture[9].Initialize("../../Assets/Images/planets/neptune.jpg");
    mDiffuseTexture[10].Initialize("../../Assets/Images/planets/pluto.jpg");
    
    for (int i = 0; i < 11; i++)
    {
        mSampler[i].Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    }

    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    for (int i = 0; i < 11; i++)
    {
        mSampler[i].Terminate();
        mDiffuseTexture[i].Terminate();
        mPixelShader[i].Terminate();
        mVertexShader[i].Terminate();
        mConstantBuffer[i].Terminate();
        mMeshBuffer[i].Terminate();
    }
}

float gRotX = 0;
float gRotY = 0;
float revSpeed = 0.01;
float orbSpeed = 0.01;
int currentRender = 0;
float renderTargetDistance = -200.0f;
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    gRotY += revSpeed * deltaTime;
    //gRotX += Math::Constants::HalfPi * deltaTime * 0.25f;
}

void GameState::Render()
{
    for (int i = 0; i < 11; i++)
    {
        mVertexShader[i].Bind();
        mPixelShader[i].Bind();
        mDiffuseTexture[i].BindPS(0);
        mSampler[i].BindPS(0);

        // constant buffer
        Matrix4 matWorld = Matrix4::RotationY(gRotY) * Matrix4::RotationX(gRotX);
        Matrix4 matView = mCamera.GetViewMatrix();
        Matrix4 matProj = mCamera.GetProjectionMatrix();
        Matrix4 matFinal = matWorld * matView * matProj;
        Matrix4 wvp = Transpose(matFinal);
        mConstantBuffer[i].Update(&wvp);
        mConstantBuffer[i].BindVS(0);

        // mesh buffer
        mMeshBuffer[i].Render();
    }

    mVertexShader[currentRender].Bind();
    mPixelShader[currentRender].Bind();
    mDiffuseTexture[currentRender].BindPS(0);
    mSampler[currentRender].BindPS(0);

    Matrix4 matWorld1 = Matrix4::RotationY(gRotY) * Matrix4::RotationX(gRotX);
    Matrix4 matView1 = mRenderTargetCamera.GetViewMatrix();
    Matrix4 matProj1 = mRenderTargetCamera.GetProjectionMatrix();
    Matrix4 matFinal1 = matWorld1 * matView1 * matProj1;
    Matrix4 wvp1 = Transpose(matFinal1);
    mConstantBuffer[currentRender].Update(&wvp1);
    mConstantBuffer[currentRender].BindVS(0);

    mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, renderTargetDistance });
    mRenderTarget.BeginRender();
    mMeshBuffer[currentRender].Render();
    mRenderTarget.EndRender();

    //SimpleDraw::Render(mCamera);
}

void GameState::UpdateCamera(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed);
    }
    if (input->IsMouseDown(MouseButton::LBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    currentRender = static_cast<int>(mCelestials);

    if (ImGui::Combo("ShowWhichPlanet", &currentRender, gCelestials, static_cast<int>(std::size(gCelestials))))
    {
        mCelestials = (Celestials)currentRender;
    }


    //Render target distance
    ImGui::DragFloat("RenderDist", &renderTargetDistance, 0.1f,-300.0f, -2.0f);

    //Planet display
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 256, 256 },
        { 0, 0 },	//uv0
        { 1, 1 },	//uv1
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });


    //For rings and rotation speed
    ImGui::DragFloat("RevolutionSpeed", &revSpeed, 0.01f);
    ImGui::DragFloat("OrbitSpeed", &orbSpeed, 0.01f);

    /*switch (mCelestials)
    {
    
    case Celestials::None:
    default:
        break;
    }*/
    ImGui::End();
}
