#pragma once

namespace PanicEngine
{
    class GameObject;
    class GameWorld;

    namespace GameObjectFactory
    {
        void Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& world);
        void OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject);
    }
}