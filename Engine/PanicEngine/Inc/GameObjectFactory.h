#pragma once

namespace PanicEngine
{
    class GameObject;
    class GameWorld;
    class Component;

    using CustomComponentCB = std::function<Component* (const std::string&, GameObject&)>;

    namespace GameObjectFactory
    {
        void SetCustomMake(CustomComponentCB cb);
        void SetCustomGet(CustomComponentCB cb);
        void Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& world);
        void OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject);
    }
}