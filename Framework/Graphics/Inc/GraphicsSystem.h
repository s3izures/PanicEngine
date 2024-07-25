#pragma once
#include "Color.h"

namespace PanicEngine::Graphics
{
    class GraphicsSystem final
    {
    public:
        static void StaticInitialize(HWND window, bool fullscreen);
        static void SaticTerminate();
        static GraphicsSystem* Get();

        GraphicsSystem() = default;
        ~GraphicsSystem(); //Used to throw asserts

        //Because it's a singleton, delete copy and move constructors
        GraphicsSystem(const GraphicsSystem&) = delete; //Delete copy constructor
        GraphicsSystem(const GraphicsSystem&&) = delete; //Delete move constructor
        GraphicsSystem& operator=(const GraphicsSystem&) = delete;
        GraphicsSystem& operator=(const GraphicsSystem&&) = delete;

        void Initialize(HWND window, bool fullscreen);
        void Terminate();

        void BeginRender();
        void EndRender();

        void ToggleFullScreen();
        void Resize(uint32_t width, uint32_t height);
        void ResetRenderTarget();
        void ResetViewport();

        void SetClearColor(const Color& color);
        void SetVSync(bool vSync);

        uint32_t GetBackBufferWidth() const;
        uint32_t GetBackBufferHeight() const;

        float GetBackBufferAspectRatio() const;

        //DirectX
        ID3D11Device* GetDevice();
        ID3D11DeviceContext* GetContext();

    private:
        ID3D11Device* mD3DDevice = nullptr;
        ID3D11DeviceContext* mImmediateContext = nullptr;

        IDXGISwapChain* mSwapChain = nullptr;
        ID3D11RenderTargetView* mRenderTargetView = nullptr;

        ID3D11Texture2D* mDepthStencilBuffer = nullptr;
        ID3D11DepthStencilView* mDepthStencilView = nullptr;

        DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
        D3D11_VIEWPORT mViewport{};

        Color mClearColor = Colors::Black;
        UINT mVSync = 1;
    };
}