#include "Precompiled.h"
#include "UISpriteComponent.h"
#include "GameWorld.h"
#include "UIRenderService.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void UISpriteComponent::Initialize()
{
    ASSERT(!mTexturePath.empty(), "UISpriteComponent: texture path is not set");
    mUISprite.Initialize(mTexturePath);
    if (mRect.right - mRect.left > 0)
    {
        mUISprite.SetRect(mRect.top, mRect.left, mRect.right, mRect.bottom);
    }
    UIRenderService* uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Register(this);
}

void UISpriteComponent::Terminate()
{
    UIRenderService* uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Unregister(this);
    mUISprite.Terminate();
}

void UISpriteComponent::Render()
{
    UISpriteRenderer::Get()->Render(&mUISprite);
}

void UISpriteComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Texture"))
    {
        mTexturePath = value["Texture"].GetString();
    }
    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }
    if (value.HasMember("Scale"))
    {
        auto scale = value["Scale"].GetArray();
        const float x = scale[0].GetFloat();
        const float y = scale[1].GetFloat();
        mUISprite.SetScale({ x,y });
    }
    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        const float r = color[0].GetFloat();
        const float g = color[1].GetFloat();
        const float b = color[2].GetFloat();
        const float a = color[3].GetFloat();
        mUISprite.SetColor({ r,g,b,a });
    }
    if (value.HasMember("Rotation"))
    {
        const float rotation = value["Rotation"].GetFloat();
        mUISprite.SetRotation(rotation);
    }
    if (value.HasMember("Rect"))
    {
        auto rect = value["Rect"].GetArray();
        mRect.top = rect[0].GetFloat();
        mRect.left = rect[1].GetFloat();
        mRect.right = rect[2].GetFloat();
        mRect.bottom = rect[3].GetFloat();
    }
    if (value.HasMember("Pivot"))
    {
        std::string pivot = value["Pivot"].GetString();
        if (pivot == "TopLeft")         { mUISprite.SetPivot(Pivot::TopLeft); }
        else if (pivot == "Top")        { mUISprite.SetPivot(Pivot::Top); }
        else if (pivot == "TopRight")   { mUISprite.SetPivot(Pivot::TopRight); }
        else if (pivot == "Left")       { mUISprite.SetPivot(Pivot::Left); }
        else if (pivot == "Center")     { mUISprite.SetPivot(Pivot::Center); }
        else if (pivot == "Right")      { mUISprite.SetPivot(Pivot::Right); }
        else if (pivot == "BottomLeft") { mUISprite.SetPivot(Pivot::BottomLeft); }
        else if (pivot == "Bottom")     { mUISprite.SetPivot(Pivot::Bottom); }
        else if (pivot == "BottomRight"){ mUISprite.SetPivot(Pivot::BottomRight); }
        else { ASSERT(false, "UISpriteComponent: invalid pivot %s", pivot.c_str()); }
    }
    if (value.HasMember("Flip"))
    {
        std::string flip = value["Flip"].GetString();
        if (flip == "None")             { mUISprite.SetFlip(Flip::None); }
        else if (flip == "Horizontal")  { mUISprite.SetFlip(Flip::Horizontal); }
        else if (flip == "Vertical")    { mUISprite.SetFlip(Flip::Vertical); }
        else if (flip == "Both")        { mUISprite.SetFlip(Flip::Both); }
        else { ASSERT(false, "UiSpriteComponent: invalid flip %s", flip.c_str()); }
    }
}

Math::Vector2 UISpriteComponent::GetPosition(bool includeOrigin)
{
    float x = 0.0f;
    float y = 0.0f;
    if (includeOrigin)
    {
        mUISprite.GetOrigin(x, y);
    }
    return { mPosition.x - x, mPosition.y - y };
}
