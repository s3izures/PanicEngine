#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Math;

namespace
{
    std::unique_ptr<GraphicsSystem> sGraphicsSystem;
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
    ASSERT(sGraphicsSystem == nullptr, "GraphicsSystem: is already initialized");
    sGraphicsSystem = std::make_unique<GraphicsSystem>();
    sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::SaticTerminate()
{
    if (sGraphicsSystem != nullptr)
    {
        sGraphicsSystem-> Terminate();
        sGraphicsSystem.reset();
    }
}

GraphicsSystem* GraphicsSystem::Get()
{
    ASSERT(sGraphicsSystem != nullptr, "GraphicsSystem: was not initialized");
    return sGraphicsSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
    ASSERT(mD3DDevice == nullptr, "GraphicsSystem;terminate must be called");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
    RECT clientRect{};
    GetClientRect(window, &clientRect);
    UINT width = (UINT)(clientRect.right - clientRect.left);
    UINT height = (UINT)(clientRect.bottom - clientRect.top);

    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = !fullscreen;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &mSwapChain,
        &mD3DDevice,
        nullptr,
        &mImmediateContext
    );

    ASSERT(SUCCEEDED(hr), "GraphicsSystem:failed to create device or swap chain");
    mSwapChain->GetDesc(&mSwapChainDesc);

    Resize(GetBackBufferWidth(), GetBackBufferHeight());
}

void GraphicsSystem::Terminate()
{
    SafeRelease(mDepthStencilView);
    SafeRelease(mDepthStencilBuffer);
    SafeRelease(mRenderTargetView);
    SafeRelease(mSwapChain);
    SafeRelease(mImmediateContext);
    SafeRelease(mD3DDevice);
}

void GraphicsSystem::BeginRender()
{
    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
    mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
    mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void GraphicsSystem::EndRender()
{
    mSwapChain->Present(mVSync, 0);
}

void GraphicsSystem::ToggleFullScreen()
{
    BOOL fullscreen;
    mSwapChain->GetFullscreenState(&fullscreen, nullptr);
    mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
}

void GraphicsSystem::ResetRenderTarget()
{
    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicsSystem::ResetViewport()
{
    mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::SetClearColor(const Color& color)
{
    mClearColor = color;
}

void GraphicsSystem::SetVSync(bool vSync)
{
    mVSync = vSync ? 1 : 0;
}

uint32_t GraphicsSystem::GetBackBufferWidth() const
{
    return mSwapChainDesc.BufferDesc.Width;
}

uint32_t GraphicsSystem::GetBackBufferHeight() const
{
    return mSwapChainDesc.BufferDesc.Height;
}

float GraphicsSystem::GetBackBufferAspectRatio() const
{
    return static_cast<float>(GetBackBufferWidth()) / static_cast<float>(GetBackBufferHeight());
}

ID3D11Device* GraphicsSystem::GetDevice()
{
    return mD3DDevice;
}

ID3D11DeviceContext* GraphicsSystem::GetContext()
{
    return mImmediateContext;
}
