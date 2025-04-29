#include "Precompiled.h"
#include "ModelComponent.h"

using namespace PanicEngine;

void ModelComponent::Initialize()
{
    Graphics::ModelCache* mc = Graphics::ModelCache::Get();
    mModelId = mc->GetModelId(mFileName);
    if (mc->GetModel(mModelId) == nullptr)
    {
        mModelId = mc->LoadModel(mFileName);
        for (const std::string& animation : mAnimations)
        {
            mc->AddAnimation(mModelId, animation);
        }
    }

    ASSERT(mc->GetModel(mModelId) != nullptr, "ModelComponent: model is not null %s", mFileName.c_str());
}

void ModelComponent::Terminate()
{
    RenderObjectComponent::Terminate();
}

void ModelComponent::Deserialize(const rapidjson::Value& value)
{
    RenderObjectComponent::Deserialize(value);
    if (value.HasMember("FileName"))
    {
        mFileName = value["FileName"].GetString();
    }
    if (value.HasMember("Animations"))
    {
        mAnimations.clear();
        auto animations = value["Animations"].GetArray();
        for (auto& animation : animations)
        {
            mAnimations.push_back(animation.GetString());
        }
    }
}

Graphics::ModelId ModelComponent::GetModelId() const
{
    return mModelId;
}

const Graphics::Model& ModelComponent::GetModel() const
{
    return *Graphics::ModelCache::Get()->GetModel(mModelId);
}

