#pragma once
#include "ConstantBuffer.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace PanicEngine::Graphics
{
    class Camera;
    class Texture;
    class RenderObject;

    class TerrainEffect final
    {
    public:
        void Initialize();
        void Terminate();
        void Begin();
        void End();
        void Render(const RenderObject& renderObject);
        void DebugUI();
        
        void SetCamera(const Camera& camera);
        void SetDirectionalLight(const DirectionalLight& directionalLight);

    private:
        struct TransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 world;
            Math::Vector3 viewPosition;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            int useBlend = 0;
            float blendHeight = 0.0f;
            float blendWidth = 0.0f;
            float padding = 0.0f;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;

        TransformBuffer mTransformBuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMaterialBuffer;
        SettingsBuffer mSettingsBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;

        SettingsData mSettingsData;
        const Camera* mCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
    };
}