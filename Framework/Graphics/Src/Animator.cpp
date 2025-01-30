#include "Precompiled.h"
#include "Animator.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void Animator::Initialize(ModelId id)
{
	mModelId = id;
	mIsLoop = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}

void Animator::PlayAnimation(int clipIndex, bool loop)
{
	mClipIndex = clipIndex;
	mIsLoop = loop;
	mAnimationTick = 0.0f;
}

void Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
	{
		return;
	}

	const Model* model = ModelCache::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mAnimationTick *= animClip.ticksPerSecond * deltaTime;
	if (mAnimationTick > animClip.tickDuration)
	{
		if (mIsLoop)
		{
			while (mAnimationTick > animClip.tickDuration)
			{
				mAnimationTick -= animClip.tickDuration;
			}
		}
		else
		{
			mAnimationTick = animClip.tickDuration; //clamp
		}
	}
}

bool Animator::IsFinished() const
{
	if (mClipIndex < 0 || mIsLoop)
	{
		return false;
	}
	const Model* model = ModelCache::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}

size_t Animator::GetAnimationCount() const
{
	const Model* model = ModelCache::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

Math::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}

	const Model* model = ModelCache::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimations[bone->index].get();
	if (animation == nullptr)
	{
		return Math::Matrix4::Identity;
	}
	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix4();
}
