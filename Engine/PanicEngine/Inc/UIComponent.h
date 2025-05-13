#pragma once
#include "Component.h"

namespace PanicEngine
{
    class UIComponent : public Component
    {
    public:
        virtual void Render() = 0;
    };
}