#pragma once
#include "Component.h"

namespace PanicEngine
{
    class SoundEffectComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::SoundEffect);

        void Initialize() override;
        void Terminate() override;

        void Deserialize(const rapidjson::Value& value);
        void DebugUI() override;

        void Play();
        void Stop();

    private:
        std::string mFileName;
        bool mLooping = false;
        Audio::SoundId mSoundId;
    };
}