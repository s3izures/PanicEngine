#include "Precompiled.h"
#include "TerrainEffect.h"

#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "Camera.h"
#include "VertexTypes.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
void TerrainEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Terrain.fx";
    mVertexShader.Initialize<Vertex>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
    mSampler.Terminate();
    mSettingsBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void TerrainEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);

    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);

    mMaterialBuffer.BindPS(2);

    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);

    mSampler.BindVS(0);
    mSampler.BindPS(0);
}

void TerrainEffect::End()
{

}

void TerrainEffect::Render(const RenderObject& renderObject)
{
    ASSERT(mCamera != nullptr, "StandardEffect: must have a camera");

    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData transformData;
    transformData.wvp = Transpose(matFinal);
    transformData.world = Transpose(matWorld);
    transformData.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(transformData);

    mSettingsBuffer.Update(mSettingsData);
    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    TextureCache* tc = TextureCache::Get();
    tc->BindPS(renderObject.diffuseMapId, 0);
    tc->BindPS(renderObject.normalMapId, 1);

    renderObject.meshBuffer.Render();
}

void TerrainEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("TerrainEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool useBlend = mSettingsData.useBlend > 0;
        if (ImGui::Checkbox("UseBlend", &useBlend))
        {
            mSettingsData.useBlend = useBlend ? 1 : 0;
        }
        ImGui::DragFloat("BlendHeight", &mSettingsData.blendHeight, 0.1f, 0.0f, 20.0f);
        ImGui::DragFloat("BlendThickness", &mSettingsData.blendThickness, 0.1f, 0.0f, 20.0f);
    }
}

void TerrainEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}