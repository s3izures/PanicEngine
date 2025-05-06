#pragma once
#include "Component.h"

namespace PanicEngine
{
    class SoundBankComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::SoundBank);

        void Initialize() override;
        void Terminate() override;

        void Deserialize(const rapidjson::Value& value);
        void DebugUI() override;

        void Play(const std::string& key);
        void Stop(const std::string& key);
        void StopAll();

    private:
        struct SoundEffectData
        {
            std::string fileName;
            Audio::SoundId soundId;
            bool looping = false;
        };
        using SoundEffects = std::unordered_map<std::string, SoundEffectData>;
        SoundEffects mSoundEffects;
    };
}