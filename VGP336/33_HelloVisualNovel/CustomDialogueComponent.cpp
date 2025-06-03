#include "CustomDialogueComponent.h"
#include "CustomDialogueService.h"

void CustomDialogueComponent::Initialize()
{
    CustomDialogueService* cds = GetOwner().GetWorld().GetService<CustomDialogueService>();
    cds->Register(this);
}

void CustomDialogueComponent::Terminate()
{
    CustomDialogueService* cds = GetOwner().GetWorld().GetService<CustomDialogueService>();
    cds->Unregister(this);
}

void CustomDialogueComponent::Update(float deltaTime)
{
}

void CustomDialogueComponent::RenderText() const
{
    PanicEngine::Graphics::UIFont::Get()->DrawString(mDialogueLines[mCurrentLineIndex].wstring().c_str(), mPosition, mColor, mSize);
}

bool CustomDialogueComponent::UpdateCurrentIndex(int index)
{
    if (index < mLineCount)
    {
        mCurrentLineIndex = index;
        return true;
    }
    return false;
}

int CustomDialogueComponent::GetCurrentIndex()
{
    return mCurrentLineIndex;
}

void CustomDialogueComponent::DebugUI()
{
    ImGui::DragInt("LineIndex", &mCurrentLineIndex, 1, 0, mLineCount - 1);
    std::string line = mDialogueLines[mCurrentLineIndex].string();
    ImGui::Text("DIALOGUE TEXT:\n%s", line.c_str());
}

void CustomDialogueComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
}

void CustomDialogueComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("DialogueLines"))
    {
        mDialogueLines.clear();
        auto dialogueLines = value["DialogueLines"].GetArray();
        for (auto& line : dialogueLines)
        {
            mDialogueLines.push_back(line.GetString());
        }

        mLineCount = dialogueLines.Size();
    }
    if (value.HasMember("TextSize"))
    {
        mSize = value["TextSize"].GetFloat();
    }
    if (value.HasMember("TextPosition"))
    {
        auto pos = value["TextPosition"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }
    if (value.HasMember("TextColor"))
    {
        auto color = value["TextColor"].GetArray();
        mColor.r = color[0].GetFloat();
        mColor.g = color[1].GetFloat();
        mColor.b = color[2].GetFloat();
        mColor.a = color[3].GetFloat();
    }
}
