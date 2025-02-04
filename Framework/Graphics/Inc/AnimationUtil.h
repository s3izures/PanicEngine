#pragma once
#include "ModelCache.h"
#include "Animator.h"

namespace PanicEngine::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<Math::Matrix4>;

	void ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator = nullptr);
	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
	void ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms);
}