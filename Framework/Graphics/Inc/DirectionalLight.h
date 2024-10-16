#pragma once
#include "Color.h"

namespace PanicEngine::Graphics
{
    struct DirectionalLight
    {
        Color ambient = Colors::White; //any light color without light contact
        Color diffuse = Colors::White; //base light color
        Color specular = Colors::White; //highlights
        Math::Vector3 direction = Math::Vector3::ZAxis;
        float padding = 0.0f;
    };
}