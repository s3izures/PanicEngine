#include "Precompiled.h"
#include "TextureCache.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

namespace
{
    std::unique_ptr<TextureCache> sInstance;
}

void TextureCache::StaticInitialize(const std::filesystem::path& root)
{
    ASSERT(sInstance == nullptr, "TextureCache: is already initiated");
    sInstance = std::make_unique<TextureCache>();
    sInstance->SetRootDirectory(root);
}

void TextureCache::StaticTerminate()
{
    sInstance.reset();
}

TextureCache* TextureCache::Get()
{
    ASSERT(sInstance != nullptr, "TextureCache: is not initialized");
    return sInstance.get();
}

TextureCache::~TextureCache()
{
    for (auto& [id, texture] : mInventory)
    {
        texture->Terminate();
    }
    mInventory.clear();
}

void TextureCache::SetRootDirectory(std::filesystem::path root)
{
    mRootDirectory = root;
}

TextureId TextureCache::LoadTexture(const std::filesystem::path& fileName, bool useRootDir)
{
    TextureId textureId = std::filesystem::hash_value(fileName);
    auto [iter, success] = mInventory.insert({ textureId,nullptr });
    if (success)
    {
        auto& texturePtr = iter->second;
        texturePtr = std::make_unique<Texture>();
        texturePtr->Initialize((useRootDir) ? mRootDirectory / fileName : fileName);
    }
    return textureId;
}

const Texture* TextureCache::GetTexture(TextureId id) const
{
    auto iter = mInventory.find(id);
    if (iter != mInventory.end())
    {
        return iter->second.get();
    }
    return nullptr;
}

void TextureCache::BindVS(TextureId id, uint32_t slot) const
{
    auto iter = mInventory.find(id);
    if (iter != mInventory.end())
    {
        iter->second->BindVS(slot);
    }
}

void TextureCache::BindPS(TextureId id, uint32_t slot) const
{
    auto iter = mInventory.find(id);
    if (iter != mInventory.end())
    {
        iter->second->BindPS(slot);
    }
}
