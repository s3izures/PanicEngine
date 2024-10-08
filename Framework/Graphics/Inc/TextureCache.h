#pragma once
#include "Texture.h"

namespace PanicEngine::Graphics
{
    using TextureId = std::size_t;
    
    class TextureCache final
    {
    public:
        static void StaticInitialize(const std::filesystem::path& root);
        static void StaticTerminate();
        static TextureCache* Get();

        TextureCache() = default;
        ~TextureCache();
        TextureCache(const TextureCache&) = delete;
        TextureCache(const TextureCache&&) = delete;
        TextureCache& operator= (const TextureCache&) = delete;
        TextureCache& operator= (const TextureCache&&) = delete;

        void SetRootDirectory(std::filesystem::path root);

        TextureId LoadTexture(const std::filesystem::path& filePath, bool useRootDir = true);
        const Texture* GetTexture(TextureId id) const;

        void BindVS(TextureId id, uint32_t slot) const;
        void BindPS(TextureId id, uint32_t slot) const;

    private:
        using Inventory = std::unordered_map<TextureId, std::unique_ptr<Texture>>;
        Inventory mInventory;

        std::filesystem::path mRootDirectory;
    };
}