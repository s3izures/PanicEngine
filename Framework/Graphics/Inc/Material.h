#pragma once
#include "Color.h"

namespace PanicEngine::Graphics
{
    struct Material
    {
        Color ambient = Colors::White; //any light color without light contact
        Color diffuse = Colors::White; //base light color
        Color specular = Colors::White; //highlights
        Color emissive = Colors::Black; //glow
        float power = 10.0f;
        float padding[3] = { 0.0f };
    };
}