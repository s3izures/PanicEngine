#pragma once
#include "UIComponent.h"

namespace PanicEngine
{
    class UISpriteComponent : public UIComponent
    {
    public:
        SET_TYPE_ID(ComponentId::UISprite);

        void Initialize() override;
        void Terminate() override;
        void Render() override;
        void Deserialize(const rapidjson::Value& value) override;

        Math::Vector2 GetPosition(bool includeOrigin = true);

    private:
        std::filesystem::path mTexturePath;
        DirectX::XMFLOAT2 mPosition;
        RECT mRect = { 0,0,0,0 };
        Graphics::UISprite mUISprite;
    };
}