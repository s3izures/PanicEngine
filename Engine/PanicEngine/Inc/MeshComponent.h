#pragma once
#include "RenderObjectComponent.h"

namespace PanicEngine
{
    class MeshComponent final : public RenderObjectComponent
    {
    public:
        SET_TYPE_ID(ComponentId::Mesh);

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original) override;
        void Deserialize(const rapidjson::Value& value) override;
        const Graphics::Model& GetModel() const override;

    private:
        Graphics::Model mModel;
    };
}