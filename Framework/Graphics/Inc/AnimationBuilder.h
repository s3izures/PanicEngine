#pragma once
#include "Animation.h"

namespace PanicEngine::Graphics
{
    class AnimationBuilder
    {
    public:
        AnimationBuilder& AddPositionKey(const Math::Vector3& pos, float time);
        AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation, float time);
        AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time);
        AnimationBuilder& AddEventKey(AnimationCallback cb, float time);

        [[nodiscard]] Animation Build();
    private:
        Animation mWorkingCopy;
    };
}