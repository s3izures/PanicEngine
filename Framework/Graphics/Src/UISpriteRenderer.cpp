#include "Precompiled.h"
#include "UISpriteRenderer.h"

#include "UISprite.h"
#include "GraphicsSystem.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

namespace
{
    std::unique_ptr<UISpriteRenderer> sSpriteRenderer;
}

void UISpriteRenderer::StaticInitialize()
{
    ASSERT(sSpriteRenderer == nullptr, "UISpriteRenderer: is already initialized");
    sSpriteRenderer = std::make_unique< UISpriteRenderer>();
    sSpriteRenderer->Initialize();
}

void UISpriteRenderer::StaticTerminate()
{
    if (sSpriteRenderer != nullptr)
    {
        sSpriteRenderer->Terminate();
        sSpriteRenderer.reset();
    }
}

UISpriteRenderer* UISpriteRenderer::Get()
{
    ASSERT(sSpriteRenderer != nullptr, "UISpriteRenderer: is not initialized");
    return sSpriteRenderer.get();
}

UISpriteRenderer::~UISpriteRenderer()
{
    ASSERT(mSpriteBatch == nullptr, "UISpriteRenderer: terminate must be called");
}

void UISpriteRenderer::Initialize()
{
    GraphicsSystem* gs = GraphicsSystem::Get();
    mCommonStates = new DirectX::CommonStates(gs->GetDevice());
    mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
}

void UISpriteRenderer::Terminate()
{
    if (mCommonStates != nullptr)
    {
        delete mCommonStates;
        mCommonStates = nullptr;
    }
    if (mSpriteBatch != nullptr)
    {
        delete mSpriteBatch;
        mSpriteBatch = nullptr;
    }
}

void UISpriteRenderer::BeginRender()
{
    mSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, mCommonStates->NonPremultiplied());
}

void UISpriteRenderer::EndRender()
{
    mSpriteBatch->End();

    //restore state object
    auto blendState = mCommonStates->Opaque();
    auto depthStencilState = mCommonStates->DepthDefault();
    auto rasterizerState = mCommonStates->CullCounterClockwise();

    auto context = GraphicsSystem::Get()->GetContext();
    context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(depthStencilState, 0);
    context->RSSetState(rasterizerState);
}

void UISpriteRenderer::Render(const UISprite* uiSprite)
{
    const Texture* texture = TextureCache::Get()->GetTexture(uiSprite->mTextureId);
    if (texture != nullptr)
    {
        mSpriteBatch->Draw(
            (ID3D11ShaderResourceView*)texture->GetRawData(),
            uiSprite->mPosition,
            &uiSprite->mRect,
            uiSprite->mColor,
            uiSprite->mRotation,
            uiSprite->mOrigin,
            uiSprite->mScale,
            uiSprite->mFlip
        );
    }
}
