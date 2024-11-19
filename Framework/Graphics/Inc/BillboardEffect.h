#pragma once

#include "BlendState.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Sampler.h"

namespace PanicEngine::Graphics
{
    class Camera;
    class RenderObject;

    class BillboardEffect
    {
    public:
        void Initialize();
        void Terminate();
        void Begin();
        void End();
        void Render(const RenderObject& renderObject);
        void SetCamera(const Camera& camera);

    private:
        const Camera* mCamera = nullptr;
        
        struct TransformData
        {
            Math::Matrix4 wvp;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        TransformBuffer mTransformBuffer;
        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;
        BlendState mBlendState;
    };
}