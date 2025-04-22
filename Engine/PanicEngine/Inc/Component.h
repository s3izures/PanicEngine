#pragma once
#include "TypeIds.h"

namespace PanicEngine
{
    class GameObject;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(const Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(const Component&&) = delete;

        virtual uint32_t GetTypeId() const = 0;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltaTime) {}
        virtual void DebugUI() {}

        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) {} //Saving
        virtual void Deserialize( const rapidjson::Value& value) {} //Loading

        GameObject& GetOwner() { return *mOwner; }
        const GameObject& GetOwner() const { return *mOwner; }

    private:
        friend class GameObject;
        GameObject* mOwner = nullptr;
    };
}