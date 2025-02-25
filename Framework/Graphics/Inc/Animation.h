#pragma once
#include "Keyframe.h"
#include "Transform.h"

namespace PanicEngine::Graphics
{
    class Animation
    {
    public:
        Transform GetTransform(float time) const;
        float GetDuration() const;

        void PlayEvents(float prevTime, float curTime);

    private:
        Math::Vector3 GetPosition(float time) const;
        Math::Quaternion GetRotation(float time) const;
        Math::Vector3 GetScale(float time) const;

        friend class AnimationBuilder;
        friend class AnimationIO;

        PositionKeys mPositionKeys;
        RotationKeys mRotationKeys;
        ScaleKeys mScaleKeys;
        EventKeys mEventKeys;
        float mDuration;
    };
}