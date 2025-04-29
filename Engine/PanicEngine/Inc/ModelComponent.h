#pragma once
#include "RenderObjectComponent.h"

namespace PanicEngine
{
    class ModelComponent final : public RenderObjectComponent
    {
    public:
        SET_TYPE_ID(ComponentId::Model);

        void Initialize() override;
        void Terminate() override;

        void Deserialize(const rapidjson::Value& value) override;

        Graphics::ModelId GetModelId() const override;
        const Graphics::Model& GetModel() const override;

        const std::vector<const Component*> GetStuff(const rapidjson::Value& v)const;

    private:
        std::string mFileName;
        Graphics::ModelId mModelId;

        using Animations = std::vector<std::string>;
        Animations mAnimations;
    };
}