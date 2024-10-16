#include "Precompiled.h"
#include "StandardEffect.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
    mVertexShader.Initialize<Vertex>(path);
    mPixelShader.Initialize(path);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();
}

void StandardEffect::Terminate()
{
    mSettingsBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void StandardEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    mTransformBuffer.BindVS(0);

    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);

    mMaterialBuffer.BindPS(2);

    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);
}

void StandardEffect::End()
{
    //shadow release texture
    //blends clear blend state
}

void StandardEffect::Render(const RenderObject& renderObject)
{
    ASSERT(mCamera != nullptr, "StandardEffect: must have a camera");

    SettingsData settingsData;
    settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0;
    settingsData.useDiffuseMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
    settingsData.useDiffuseMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0;
    settingsData.useDiffuseMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0;
    mSettingsBuffer.Update(settingsData);

    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData transformData;
    transformData.wvp = Transpose(matFinal);
    transformData.world = Transpose(matWorld);
    transformData.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(transformData);

    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    TextureCache* tc = TextureCache::Get();
    tc->BindPS(renderObject.diffuseMapId, 0);
    renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}

void StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool useDiffuse = mSettingsData.useDiffuseMap > 0;
        if (ImGui::Checkbox("useDiffuse", &useDiffuse))
        {
            mSettingsData.useDiffuseMap = (useDiffuse) ? 1 : 0;
        }
        bool useNormal = mSettingsData.useNormalMap > 0;
        if (ImGui::Checkbox("useNormal", &useNormal))
        {
            mSettingsData.useNormalMap = (useNormal) ? 1 : 0;
        }
        bool useSpec = mSettingsData.useSpecMap > 0;
        if (ImGui::Checkbox("useSpec", &useSpec))
        {
            mSettingsData.useSpecMap = (useSpec) ? 1 : 0;
        }
        bool useBump = mSettingsData.useBumpMap > 0;
        if (ImGui::Checkbox("useBump", &useBump))
        {
            mSettingsData.useBumpMap = (useBump) ? 1 : 0;
        }
    }
}
