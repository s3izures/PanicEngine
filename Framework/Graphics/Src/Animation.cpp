#include "Precompiled.h"
#include "Animation.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

namespace
{
	float GetLerpTime(float startTime, float endTime, float time)
	{
		float t = (time - startTime) / (endTime - startTime);
		return Math::Clamp(t, 0.0f, 1.0f);
	}
}

Transform Animation::GetTransform(float time) const
{
	Transform transform;
	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);
	return transform;
}

float Animation::GetDuration() const
{
	return mDuration;
}

Math::Vector3 Animation::GetPosition(float time) const
{
	if (mPositionKeys.empty())
	{
		return Math::Vector3::Zero;
	}

	for (uint32_t i = 1; i < mPositionKeys.size(); ++i)
	{
		if (time < mPositionKeys[i].time)
		{
			float t = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time);
			return Math::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, t);
		}
	}

	return mPositionKeys.back().key;
}

Math::Quaternion Animation::GetRotation(float time) const
{
	if (mRotationKeys.empty())
	{
		return Math::Quaternion::Identity;
	}

	for (uint32_t i = 1; i < mRotationKeys.size(); ++i)
	{
		if (time < mRotationKeys[i].time)
		{
			float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time);
			return Math::Quaternion::Slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, t);
		}
	}

	return mRotationKeys.back().key;
}

Math::Vector3 Animation::GetScale(float time) const
{
	if (mScaleKeys.empty())
	{
		return Math::Vector3::One;
	}

	for (uint32_t i = 1; i < mScaleKeys.size(); ++i)
	{
		if (time < mScaleKeys[i].time)
		{
			float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time);
			return Math::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
		}
	}

	return mScaleKeys.back().key;
}
