#pragma once
#include "Common.h"

namespace PanicEngine::Graphics
{
    struct Bone
    {
        std::string name;
        int index = -1;

        Bone* parent = nullptr;
        int parentIndex = -1;

        std::vector<Bone*> children;
        std::vector<int> childrenIndices;

        Math::Matrix4 toParentTransform;    // local transform
        Math::Matrix4 offsetTransform;        // more for skinning. Offset from T-pose. Offset from base pose of the model
    };
}