#include "Precompiled.h"
#include "GameObject.h"

using namespace PanicEngine;

static uint32_t gUniqueId = 0;	// g is for global

void GameObject::Initialize()
{
    ASSERT(!mInitialized, "GameObject: is already initialized");
    for (auto& component : mComponents)
    {
        component->Initialize();
    }

    mUniqueId = ++gUniqueId;
    mInitialized = true;

    for (GameObject* child : mChildren)
    {
        child->Initialize();
    }
}

void GameObject::Terminate()
{
    for (GameObject* child : mChildren)
    {
        child->Terminate();
    }
    for (auto& component : mComponents)
    {
        component->Terminate();
        component.reset();
    }
    mComponents.clear();
}

void GameObject::Update(float deltaTime)
{
    for (auto& component : mComponents)
    {
        component->Update(deltaTime);
    }
}

void GameObject::DebugUI()
{
    ImGui::PushID(mUniqueId);
    if (ImGui::CollapsingHeader(mName.c_str()))
    {
        for (auto& component : mComponents)
        {
            component->DebugUI();
        }
    }
    ImGui::PopID();
}

void GameObject::SetName(std::string& name)
{
    mName = std::move(name);
}

const std::string& GameObject::GetName() const
{
    return mName;
}

uint32_t GameObject::GetUniqueId() const
{
    return mUniqueId;
}

GameWorld& GameObject::GetWorld()
{
    return *mWorld;
}

const GameWorld& GameObject::GetWorld() const
{
    return *mWorld;
}

const GameObjectHandle& GameObject::GetHandle() const
{
    return mHandle;
}

void GameObject::AddChild(GameObject* child)
{
    mChildren.push_back(child);
}

GameObject* GameObject::GetChild(uint32_t index)
{
    return mChildren[index];
}

const GameObject* GameObject::GetChild(uint32_t index) const
{
    return mChildren[index];
}

void GameObject::SetParent(GameObject* parent)
{
    mParent = parent;
}

GameObject* GameObject::GetParent()
{
    return mParent;
}

const GameObject* GameObject::GetParent() const
{
    return mParent;
}