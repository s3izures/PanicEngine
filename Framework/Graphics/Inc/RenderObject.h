#pragma once
#include "MeshBuffer.h"
#include "Texture.h"

namespace PanicEngine::Graphics 
{
    class RenderObject
    {
    public:
        void Terminate();

        MeshBuffer meshBuffer;
        Texture diffuseTexture;

    private:


    };
}