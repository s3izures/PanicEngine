#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"


using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void RenderObject::Terminate()
{
    meshBuffer.Terminate();
}

void RenderGroup::Terminate()
{
    for (RenderObject& renderObject : renderObjects)
    {
        renderObject.Terminate();
    }
}

void RenderGroup::Initialize(const Model& model)
{
    for (const Model::MeshData& meshData : model.meshData)
    {
        RenderObject& renderObject = renderObjects.emplace_back();
        renderObject.meshBuffer.Initialize(meshData.mesh);
    }
}
