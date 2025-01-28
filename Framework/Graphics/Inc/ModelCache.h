#pragma once
#include "Model.h"

namespace PanicEngine::Graphics
{
    using ModelId = std::size_t;
    
    class ModelCache final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static ModelCache* Get();

        ModelCache() = default;
        ~ModelCache() = default;

        ModelCache(const ModelCache&) = delete;
        ModelCache(const ModelCache&&) = delete;
        ModelCache& operator=(const ModelCache&) = delete;
        ModelCache&& operator=(const ModelCache&&) = delete;

        ModelId GetModelId(const std::filesystem::path& filePath);
        ModelId LoadModel(const std::filesystem::path& filePath);
        void AddAnimation(ModelId& id, const std::filesystem::path& filePath);
        const Model* GetModel(ModelId id);

    private:
        using Inventory = std::map<ModelId, std::unique_ptr<Model>>;
        Inventory mInventory;
    };
}