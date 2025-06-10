#include "CustomDialogueService.h"
#include "CustomDialogueComponent.h"

void CustomDialogueService::Render()
{
    for (const CustomDialogueComponent* component : mDialogueComponents)
    {
        component->RenderText();
    }
}

void CustomDialogueService::Register(const CustomDialogueComponent* dialogueComponent)
{
    auto iter = std::find(mDialogueComponents.begin(), mDialogueComponents.end(), dialogueComponent);
    if (iter == mDialogueComponents.end())
    {
        mDialogueComponents.push_back(dialogueComponent);
    }
}

void CustomDialogueService::Unregister(const CustomDialogueComponent* dialogueComponent)
{
    auto iter = std::find(mDialogueComponents.begin(), mDialogueComponents.end(), dialogueComponent);
    if (iter != mDialogueComponents.end())
    {
        mDialogueComponents.erase(iter);
    }
}
