#pragma once
#include "MeshBuffer.h"
#include "TextureCache.h"
#include "Transform.h"

namespace PanicEngine::Graphics 
{
    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;
        TextureId diffuseTextureId;
    };
}