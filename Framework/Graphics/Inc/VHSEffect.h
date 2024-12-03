#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace PanicEngine::Graphics
{
    class RenderObject;
    class VHSEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);
        void SetSourceTexture(const Texture& texture);
        void DebugUI();

    private:
        struct SettingsData
        {
            float abberationIntensity = 0.0f;
            float scanlineIntensity = 0.0f;
            float scanlineThickness = 0.0f;
            float noiseIntensity = 0.0f;
            float noiseSpeed = 0.0f;
            float padding[3] = { 0.0f };
        };

        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        SettingsBuffer mSettingsBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;

        const Texture* mSourceTexture = nullptr;

        float mAberrationValue = 0.005f;
        float mScanlineIntensity = 5.0f;
        float mScanlineThickness = 800.0f;
        float mNoiseIntensity = 0.05f;
        float mNoiseSpeed = 20.0f;
    };
}