#include "Precompiled.h"
#include "Texture.h"
#include "GraphicsSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void Texture::UnbindPS(uint32_t slot)
{
    static ID3D11ShaderResourceView* dummy = nullptr;
    GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
    ASSERT(mShaderResourceView == nullptr, "texture: must call terminate");
}

Texture::Texture(Texture&& rhs) noexcept
    : mShaderResourceView(rhs.mShaderResourceView)
{
    rhs.mShaderResourceView = nullptr;
}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
    mShaderResourceView = rhs.mShaderResourceView;
    rhs.mShaderResourceView = nullptr;
    return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
    auto device = GraphicsSystem::Get()->GetDevice();
    auto context = GraphicsSystem::Get()->GetContext();
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
    ASSERT(SUCCEEDED(hr), "Texture: failed to create texture %s", fileName.c_str());
}

void Texture::Terminate()
{
    SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void* PanicEngine::Graphics::Texture::GetRawData() const
{
    return mShaderResourceView;
}
