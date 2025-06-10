#include "CustomIncrementDialogueButtonComponent.h"

namespace {
    PanicEngine::UIButtonComponent* mUiButton = nullptr;
    CustomDialogueComponent* mDialogue = nullptr;
}

void IncrementDialogueIndex()
{
    int index = mDialogue->GetCurrentIndex();
    index++;
    mDialogue->UpdateCurrentIndex(index);
}

void CustomIncrementDialogueButtonComponent::Initialize()
{
    mUiButton = GetOwner().GetComponent<UIButtonComponent>();
    mDialogue = GetOwner().GetParent()->GetComponent<CustomDialogueComponent>();
    mUiButton->SetCallback(IncrementDialogueIndex);
}