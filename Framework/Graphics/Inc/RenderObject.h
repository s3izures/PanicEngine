#pragma once
#include "MeshBuffer.h"
#include "TextureCache.h"
#include "Transform.h"
#include "Material.h"
#include "ModelCache.h"
#include "Animator.h"

namespace PanicEngine::Graphics 
{
    struct Model;

    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        Material material;
        TextureId diffuseMapId;
        TextureId normalMapId;
        TextureId specMapId;
        TextureId bumpMapId;
    };

    class RenderGroup
    {
    public:
        void Initialize(const Model& model, const Animator* anim = nullptr);
        void Initialize(const std::filesystem::path& modelFile, const Animator* anim = nullptr);
        void Terminate();

        ModelId modelId;
        Transform transform;
        std::vector<RenderObject> renderObjects;
        const Skeleton* skeleton = nullptr;
        const Animator* animator = nullptr;
    };
}