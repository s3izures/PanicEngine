#pragma once
#include "CustomTypeId.h"

using namespace PanicEngine;

class CustomDialogueComponent : public PanicEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CustomDialogueComponent);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void DebugUI() override;

    void RenderText() const;
    bool UpdateCurrentIndex(int index);
    int GetCurrentIndex();

    void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original) override;
    void Deserialize(const rapidjson::Value& value) override;

private:
    Math::Vector2 mPosition = Math::Vector2::Zero;
    float mSize = 1.0f;
    Color mColor = Colors::Black;

    using DialogueLines = std::vector<std::filesystem::path>;
    DialogueLines mDialogueLines;

    int mCurrentLineIndex = 0;
    int mLineCount = 0;
};