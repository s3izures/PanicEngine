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
        void Update(float deltaTime);

        void Render(const RenderObject& renderObject);
        void SetSourceTexture(const Texture& texture);
        void SetStaticTexture(const Texture& texture, int index);
        void DebugUI();

    private:
        struct SettingsData
        {
            float abberationIntensity = 0.0f;
            float scanlineIntensity = 0.0f;
            float scanlineDensity = 0.0f;
            float noiseIntensity = 0.0f;
            float distortionIntensity = 0.0f;
            float padding[3] = { 0.0f };
        };

        using SettingsBuffer = TypedConstantBuffer<SettingsData>;
        SettingsBuffer mSettingsBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;

        const Texture* mSourceTexture = nullptr;
        // list of textures for static (at least 2)
        const Texture* mStaticTextures[4] = { nullptr };

        float mAberrationValue = 0.005f;
        float mScanlineIntensity = 0.5f;
        float mScanlineDesnity = 900.0f;
        float mNoiseIntensity = 0.4f;
        float mNoiseSpeed = 0.123f;
        int mStaticIndex = 0;
        float mNextStaticUpdate = 0.0f;
        float mDistortionIntensity = 0.5f;
    };
}