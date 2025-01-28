#pragma once
#include "Animation.h"

namespace PanicEngine::Graphics
{
	struct AnimationClip
	{
		std::string name;
		float tickDuration = 0.0f;
		float ticksPerSecond = 0.0f;

		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}