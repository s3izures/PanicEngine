#include "Precompiled.h"
#include "SoundBankComponent.h"

using namespace PanicEngine;
using namespace PanicEngine::Audio;

void SoundBankComponent::Initialize()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    for (auto& data : mSoundEffects)
    {
        data.second.soundId = sm->Load(data.second.fileName);
    }
}

void SoundBankComponent::Terminate()
{
    StopAll();
}

void SoundBankComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
}

void SoundBankComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("SoundEffects"))
    {
        auto soundEffects = value["SoundEffects"].GetObj();
        for (auto& effect : soundEffects)
        {
            SoundEffectData& data = mSoundEffects[effect.name.GetString()];
            if (effect.value.HasMember("FileName"))
            {
                data.fileName = effect.value["FileName"].GetString();
            }
            if (effect.value.HasMember("Looping"))
            {
                data.looping = effect.value["Looping"].GetBool();
            }
        }
    }
}

void SoundBankComponent::DebugUI()
{
    if (ImGui::CollapsingHeader("SoundEffects"))
    {
        ImGui::Indent();
        for (auto& effect : mSoundEffects)
        {
            ImGui::PushID(effect.first.c_str());
            ImGui::Text(effect.first.c_str());
            ImGui::Text(effect.second.fileName.c_str());
            if (ImGui::Button("Play"))
            {
                Play(effect.first);
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop"))
            {
                Stop(effect.first);
            }
            ImGui::PopID();
        }
        ImGui::Unindent();
    }
}

void SoundBankComponent::Play(const std::string& key)
{
    auto iter = mSoundEffects.find(key);
    if (iter != mSoundEffects.end())
    {
        SoundEffectManager::Get()->Play(iter->second.soundId, iter->second.looping);
    }
}

void SoundBankComponent::Stop(const std::string& key)
{
    auto iter = mSoundEffects.find(key);
    if (iter != mSoundEffects.end())
    {
        SoundEffectManager::Get()->Stop(iter->second.soundId);
    }
}

void SoundBankComponent::StopAll()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    for (auto& effect : mSoundEffects)
    {
        sm->Stop(effect.second.soundId);
    }
}
