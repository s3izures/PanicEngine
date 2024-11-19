#include "Precompiled.h"
#include "BillboardEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "TextureCache.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void BillboardEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFile);
    mPixelShader.Initialize(shaderFile);
    mTransformBuffer.Initialize();
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void BillboardEffect::Terminate()
{
    mBlendState.Terminate();
    mSampler.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void BillboardEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mBlendState.Set();
    mTransformBuffer.BindVS(0);
    mSampler.BindPS(0);
}

void BillboardEffect::End()
{
    Texture::UnbindPS(0);
    BlendState::ClearState();
}

void BillboardEffect::Render(const RenderObject& renderObject)
{
    Math::Matrix4 matView = mCamera->GetViewMatrix();
    Math::Matrix4 matWV = Math::Matrix4::Translation(Math::TransformCoord(renderObject.transform.position, matView));
    Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    
    TransformData data;
    data.wvp = Transpose(matWV * matProj);
    mTransformBuffer.Update(data);

    TextureCache* tc = TextureCache::Get();
    tc->BindPS(renderObject.diffuseMapId, 0);
    renderObject.meshBuffer.Render();
}

void BillboardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}
