#include "Precompiled.h"
#include "SoundEffectComponent.h"

using namespace PanicEngine;
using namespace PanicEngine::Audio;

void SoundEffectComponent::Initialize()
{
    ASSERT(!mFileName.empty(), "SoundEffectComponent: no sound file loaded");
    mSoundId = SoundEffectManager::Get()->Load(mFileName);
}

void SoundEffectComponent::Terminate()
{
    Stop();
}

void SoundEffectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
}

void SoundEffectComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("FileName"))
    {
        mFileName = value["FileName"].GetString();
    }
    if (value.HasMember("Looping"))
    {
        mLooping = value["Looping"].GetBool();
    }
}

void SoundEffectComponent::DebugUI()
{
    ImGui::PushID(mFileName.c_str());
    ImGui::Text(mFileName.c_str());
    if (ImGui::Button("Play"))
    {
        Play();
    }
    ImGui::SameLine(); //Play and Stop on same line
    if (ImGui::Button("Stop"))
    {
        Stop();
    }
    ImGui::PopID();
}

void SoundEffectComponent::Play()
{
    SoundEffectManager::Get()->Play(mSoundId, mLooping);
}

void SoundEffectComponent::Stop()
{
    SoundEffectManager::Get()->Stop(mSoundId);
}
