#include "Precompiled.h"
#include "RenderObject.h"


using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void RenderObject::Terminate()
{
    meshBuffer.Terminate();
    diffuseTexture.Terminate();
}
