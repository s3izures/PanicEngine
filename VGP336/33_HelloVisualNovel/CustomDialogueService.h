#pragma once
#include "CustomTypeId.h"

class CustomDialogueComponent;

class CustomDialogueService : public PanicEngine::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomDialogueService);
    void Render() override;
    void Register(const CustomDialogueComponent* dialogueComponent);
    void Unregister(const CustomDialogueComponent* dialogueComponent);

    void RegisterButton(const PanicEngine::UIButtonComponent* uiButton);

private:
    using CustomDialogueComponents = std::vector<const CustomDialogueComponent*>;
    CustomDialogueComponents mDialogueComponents;

    const PanicEngine::UIButtonComponent* mUiButton = nullptr;
};