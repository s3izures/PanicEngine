#pragma once
#include "Service.h"

namespace PanicEngine
{
    class UIComponent;

    class UIRenderService final : public Service
    {
    public:
        SET_TYPE_ID(ServiceId::UIRender);

        void Terminate() override;
        void Render() override;

        void Register(UIComponent* uiComponent);
        void Unregister(UIComponent* uiComponent);

    private:
        using UIComponents = std::vector<UIComponent*>;
        UIComponents mUIComponents;
    };
}