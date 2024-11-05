#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Sampler.h"

namespace PanicEngine::Graphics
{
    class RenderObject;
    class Texture;

    class PostProcessEffect
    {
    public:
        enum class Mode
        {
            None,
            Monochrome,
            Invert,
            Mirror,
            Blur,
            Combine2,
            MotionBlur,
            ChromaticAbberation
        };

        void Initialize(const std::filesystem::path& filePath);
        void Terminate();
        void Begin();
        void End();

        void Render(const RenderObject& renderObject);

        void SetTexture(const Texture* texture, uint32_t slot = 0);
        void SetMode(Mode mode);

        void DebugUI();

    private:
        struct PostProcessData
        {
            int mode = 0;
            float param0 = 0.0f;
            float param1 = 0.0f;
            float param2 = 0.0f;
        };

        using PostProcessingBuffer = TypedConstantBuffer<PostProcessData>;
        PostProcessingBuffer mPostProcessBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;
        std::array<const Texture*, 4> mTextures;

        Mode mMode = Mode::None;

        float mMirrorScaleX = -1.0f;
        float mMirrorScaleY = -1.0f;
        float mBlurStrength = 5.0f;
        float mAberrationValue = 0.005f;
    };
}