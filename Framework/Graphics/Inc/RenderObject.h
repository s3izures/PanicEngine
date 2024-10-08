#pragma once
#include "MeshBuffer.h"
#include "Texture.h"
#include "Transform.h"

namespace PanicEngine::Graphics 
{
    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;
        Texture diffuseTexture;
    };
}