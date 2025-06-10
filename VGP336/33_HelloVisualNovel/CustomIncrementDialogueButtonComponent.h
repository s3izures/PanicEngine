#pragma once
#include "CustomTypeId.h"
#include "CustomDialogueComponent.h"

using namespace PanicEngine;

class CustomIncrementDialogueButtonComponent : public PanicEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CustomIncrementDialogueButtonComponent);

    void Initialize() override;

    /*void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original) override;
    void Deserialize(const rapidjson::Value& value) override;*/

private:

};