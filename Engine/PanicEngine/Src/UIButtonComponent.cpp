#include "Precompiled.h"
#include "UIButtonComponent.h"
#include "GameWorld.h"
#include "UIRenderService.h"
#include "UISpriteComponent.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Input;

void UIButtonComponent::Initialize()
{
    for (ButtonStateEntry& buttonState : mButtonStates)
    {
        if (!buttonState.texture.empty())
        {
            buttonState.sprite.Initialize(buttonState.texture);
        }
    }

    UIRenderService* uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Register(this);
}

void UIButtonComponent::Terminate()
{
    UIRenderService* uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Unregister(this);

    for (ButtonStateEntry& buttonState : mButtonStates)
    {
        if (!buttonState.texture.empty())
        {
            buttonState.sprite.Terminate();
        }
    }
}

void UIButtonComponent::Update(float deltaTime)
{
    if (mCurrentState == ButtonState::Disabled)
    {
        return;
    }

    mCurrentState = ButtonState::Default;
    InputSystem* input = InputSystem::Get();
    const int mouseX = input->GetMouseScreenX();
    const int mouseY = input->GetMouseScreenY();
    if (mButtonStates[0].sprite.IsInSprite(mouseX, mouseY))
    {
        mCurrentState = ButtonState::Hover;
        if (input->IsMouseDown(MouseButton::LBUTTON))
        {
            mCurrentState = ButtonState::Click;
        }
        if (input->IsMousePressed(MouseButton::LBUTTON))
        {
            OnClick();
        }
    }
}

void UIButtonComponent::Render()
{
    uint32_t buttonState = static_cast<uint32_t>(mCurrentState);
    if (mButtonStates[buttonState].texture.empty())
    {
        buttonState = 0;
    }

    Math::Vector2 worldPosition = GetPosition(false);
    GameObject* parent = GetOwner().GetParent();
    while (parent != nullptr)
    {
        UISpriteComponent* spriteComponent = parent->GetComponent<UISpriteComponent>();
        if (spriteComponent != nullptr)
        {
            worldPosition += spriteComponent->GetPosition();
        }
        else
        {
            UIButtonComponent* buttonComponent = parent->GetComponent<UIButtonComponent>();
            if (buttonComponent != nullptr)
            {
                worldPosition += buttonComponent->GetPosition();
            }
        }
        parent = parent->GetParent();
    }

    mButtonStates[buttonState].sprite.SetPosition({ worldPosition.x, worldPosition.y });
    UISpriteRenderer::Get()->Render(mButtonStates[buttonState].sprite);
}

void UIButtonComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
}

void UIButtonComponent::Deserialize(const rapidjson::Value& value)
{
    const uint32_t buttonStateCount = static_cast<uint32_t>(ButtonState::Count);
    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }
    if (value.HasMember("Rotation"))
    {
        const float rotation = value["Rotation"].GetFloat();
        for (ButtonStateEntry& b : mButtonStates)
        {
            b.sprite.SetRotation(rotation);
        }
    }
    if (value.HasMember("Pivot"))
    {
        Pivot buttonPivot = Pivot::TopLeft;
        std::string pivot = value["Pivot"].GetString();
        if (pivot == "TopLeft")
        {
            buttonPivot = Pivot::TopLeft;
        }
        else if (pivot == "Top")
        {
            buttonPivot = Pivot::Top;
        }
        else if (pivot == "TopRight")
        {
            buttonPivot = Pivot::TopRight;
        }
        else if (pivot == "Left")
        {
            buttonPivot = Pivot::Left;
        }
        else if (pivot == "Center")
        {
            buttonPivot = Pivot::Center;
        }
        else if (pivot == "Right")
        {
            buttonPivot = Pivot::Right;
        }
        else if (pivot == "BottomLeft")
        {
            buttonPivot = Pivot::BottomLeft;
        }
        else if (pivot == "Bottom")
        {
            buttonPivot = Pivot::Bottom;
        }
        else if (pivot == "BottomRight")
        {
            buttonPivot = Pivot::BottomRight;
        }
        else
        {
            ASSERT(false, "UIButtonComponent: invalid pivot %s", pivot.c_str());
        }
        for (ButtonStateEntry& b : mButtonStates)
        {
            b.sprite.SetPivot(buttonPivot);
        }
    }

    for (uint32_t i = 0; i < buttonStateCount; ++i)
    {
        std::string buttonStateStr = "";
        ButtonState state = static_cast<ButtonState>(i);
        switch (state)
        {
        case ButtonState::Default: buttonStateStr = "Default"; break;
        case ButtonState::Hover: buttonStateStr = "Hover"; break;
        case ButtonState::Click: buttonStateStr = "Click"; break;
        case ButtonState::Disabled: buttonStateStr = "Disabled"; break;
        default:
            break;
        }
        if (!value.HasMember(buttonStateStr.c_str()))
        {
            continue;
        }

        auto buttonStateObj = value[buttonStateStr.c_str()].GetObj();
        if (buttonStateObj.HasMember("Texture"))
        {
            mButtonStates[i].texture = buttonStateObj["Texture"].GetString();
        }
        if (buttonStateObj.HasMember("Scale"))
        {
            auto scale = buttonStateObj["Scale"].GetArray();
            const float x = scale[0].GetFloat();
            const float y = scale[1].GetFloat();
            mButtonStates[i].sprite.SetScale({ x, y });
        }
        if (buttonStateObj.HasMember("Color"))
        {
            auto color = buttonStateObj["Color"].GetArray();
            const float r = color[0].GetFloat();
            const float g = color[1].GetFloat();
            const float b = color[2].GetFloat();
            const float a = color[3].GetFloat();
            mButtonStates[i].sprite.SetColor({ r, g, b, a });
        }
        if (buttonStateObj.HasMember("Flip"))
        {
            std::string flip = buttonStateObj["Flip"].GetString();
            if (flip == "None")
            {
                mButtonStates[i].sprite.SetFlip(Flip::None);
            }
            else if (flip == "Horizontal")
            {
                mButtonStates[i].sprite.SetFlip(Flip::Horizontal);
            }
            else if (flip == "Vertical")
            {
                mButtonStates[i].sprite.SetFlip(Flip::Vertical);
            }
            else if (flip == "Both")
            {
                mButtonStates[i].sprite.SetFlip(Flip::Both);
            }
            else
            {
                ASSERT(false, "UISpriteComponent: invalid flip %s", flip.c_str());
            }
        }
    }
}

Math::Vector2 UIButtonComponent::GetPosition(bool includeOrigin)
{
    float x = 0.0f;
    float y = 0.0f;
    if (includeOrigin)
    {
        mButtonStates[0].sprite.GetOrigin(x, y);
    }
    return { mPosition.x - x, mPosition.y - y };
}

void UIButtonComponent::SetCallback(ButtonCallback cb)
{
    mCallback = cb;
}

void UIButtonComponent::OnClick()
{
    LOG("BUTTON WAS CLICKED");
    if (mCallback != nullptr)
    {
        mCallback();
    }
}