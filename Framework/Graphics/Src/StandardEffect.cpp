#include "Precompiled.h"
#include "StandardEffect.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
    mVertexShader.Initialize<VertexPX>(path);
    mPixelShader.Initialize(path);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mConstantBuffer.Terminate();
}

void StandardEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);

    mConstantBuffer.BindVS(0);
}

void StandardEffect::End()
{
    //shadow release texture
    //blends clear blend state
}

void StandardEffect::Render(const RenderObject& renderObject)
{
    ASSERT(mCamera != nullptr, "StandardEffect: must have a camera!");
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;
    const Math::Matrix4 wvp = Transpose(matFinal);
    mConstantBuffer.Update(&wvp);

    renderObject.diffuseTexture.BindPS(0);
    renderObject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void StandardEffect::DebugUI()
{

}
