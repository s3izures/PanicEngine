#include "Precompiled.h"
#include "AnimationUtil.h"
#include "Color.h"
#include "SimpleDraw.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

namespace
{
	void ComputeBoneTransformsRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone != nullptr)
		{
			if (animator != nullptr)
			{
				boneTransforms[bone->index] = animator->GetToParentTransform(bone);
			}
			else
			{
				boneTransforms[bone->index] = bone->toParentTransform;
			}
			boneTransforms[bone->index] = bone->toParentTransform;
			if (bone->parent != nullptr)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
			}
			for (const Bone* child : bone->children)
			{
				ComputeBoneTransformsRecursive(child, boneTransforms, animator);
			}
		}
	}
}

void AnimationUtil::ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator)
{
	const Model* model = ModelCache::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), Math::Matrix4::Identity);
		ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms, animator);
	}
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
	const Model* model = ModelCache::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		for (const auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 boneParentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddLine(bonePos, boneParentPos, Colors::Lime);
				SimpleDraw::AddSphere(10, 10, 0.02f, bonePos, Colors::Red);
			}
		}
	}
}
