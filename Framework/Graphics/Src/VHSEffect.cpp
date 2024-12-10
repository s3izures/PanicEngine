#include "Precompiled.h"
#include "VHSEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void VHSEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/VHS.fx";

    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);
    mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
    mSettingsBuffer.Initialize();
}

void VHSEffect::Terminate()
{
    mSettingsBuffer.Terminate();
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void VHSEffect::Begin()
{
    ASSERT(mSourceTexture != nullptr, "VHSEffect: SourceTexture is null!");

    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);
    mSourceTexture->BindPS(0);

    SettingsData data;

    data.abberationIntensity = mAberrationValue;
    data.scanlineIntensity = mScanlineIntensity;
    data.scanlineDensity = mScanlineDesnity;
    data.noiseIntensity = mNoiseIntensity;
    data.distortionIntensity = mDistortionIntensity;

    mSettingsBuffer.Update(data);
    mSettingsBuffer.BindPS(0);

    mStaticTextures[mStaticIndex]->BindPS(1);
}

void VHSEffect::End()
{
    GraphicsSystem* gs = GraphicsSystem::Get();
    gs->ResetRenderTarget();
    gs->ResetViewport();

    Texture::UnbindPS(0);
    Texture::UnbindPS(1);
}

void VHSEffect::Update(float deltaTime)
{
    mNextStaticUpdate += deltaTime;
    if (mNextStaticUpdate >= mNoiseSpeed)
    {
        mStaticIndex = rand() % 4;
        mNextStaticUpdate = 0;
    }
}

void VHSEffect::Render(const RenderObject& renderObject)
{
    renderObject.meshBuffer.Render();
}

void VHSEffect::SetSourceTexture(const Texture& texture)
{
    mSourceTexture = &texture;
}

void VHSEffect::SetStaticTexture(const Texture& texture, int index)
{
    mStaticTextures[index] = &texture;
}

void VHSEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("VHSEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("AbberationValue", &mAberrationValue, 0.001f, -0.1f, 0.1f);
        ImGui::DragFloat("ScanlineIntensity", &mScanlineIntensity, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat("ScanlineDensity", &mScanlineDesnity, 1.0f, 0.0f, 1000.0f);
        ImGui::DragFloat("NoiseIntensity", &mNoiseIntensity, 0.001f, 0.0f, 10.0f);
        ImGui::DragFloat("NoiseSpeed", &mNoiseSpeed, 0.0001f, 0.0f, 5.0f);
        ImGui::DragFloat("DistortionIntensity", &mDistortionIntensity, 0.001f, 0.0f, 10.0f);
    }
}
