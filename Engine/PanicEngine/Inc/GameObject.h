#pragma once

#include "Component.h"
#include "GameObjectHandle.h"

namespace PanicEngine
{
    class GameWorld;

    class GameObject final
    {
    public:
        GameObject() = default;

        void Initialize();
        void Terminate();
        void Update(float deltaTime);
        void DebugUI();

        void SetName(std::string& name);
        const std::string& GetName() const;
        uint32_t GetUniqueId() const;

        GameWorld& GetWorld();
        const GameWorld& GetWorld() const;
        const GameObjectHandle& GetHandle() const;

        void AddChild(GameObject* child);
        GameObject* GetChild(uint32_t index);
        const GameObject* GetChild(uint32_t index) const;

        void SetParent(GameObject* parent);
        GameObject* GetParent();
        const GameObject* GetParent() const;

        template<class ComponentType>
        ComponentType* AddComponent()
        {
            static_assert(std::is_base_of_v<Component, ComponentType>,
                "GameObject: ComponentType must be of type Component");

            ASSERT(!mInitialized, "GameObject: can't add components when initialized");
            ASSERT(!HasA<ComponentType>(), "GameObject: already has component type added");
            ASSERT(ComponentType::StaticGetTypeId() != static_cast<uint32_t>(ComponentId::Invalid),
                "GameObject: component has invalid id");

            auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
            newComponent->mOwner = this;
            return static_cast<ComponentType*>(newComponent.get());
        }

        template<class ComponentType>
        bool HasA()
        {
            static_assert(std::is_base_of_v<Component, ComponentType>,
                "GameObject: ComponentType must be of type Component");

            for (auto& component : mComponents)
            {
                if (component->GetTypeId() == ComponentType::StaticGetTypeId())
                {
                    return true;
                }
            }

            return false;
        }

        template<class ComponentType>
        const ComponentType* GetComponent() const
        {
            static_assert(std::is_base_of_v<Component, ComponentType>,
                "GameObject: ComponentType must be of type Component");

            for (auto& component : mComponents)
            {
                if (component->GetTypeId() == ComponentType::StaticGetTypeId())
                {
                    return static_cast<ComponentType*>(component.get());
                }
            }

            return nullptr;
        }

        template<class ComponentType>
        ComponentType* GetComponent()
        {
            const GameObject* thisConst = static_cast<const GameObject*>(this);
            return const_cast<ComponentType*>(thisConst->GetComponent<ComponentType>());	// const_cast reverts the const
        }

    private:
        friend class GameWorld;

        std::string mName = "EMPTY";
        bool mInitialized = false;
        uint32_t mUniqueId = 0;
        GameWorld* mWorld = nullptr;
        GameObjectHandle mHandle;

        using Components = std::vector<std::unique_ptr<Component>>;
        Components mComponents;

        using Children = std::vector<GameObject*>;
        Children mChildren;
        GameObject* mParent = nullptr;
    };
}