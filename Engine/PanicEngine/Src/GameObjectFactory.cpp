#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"
#include "GameWorld.h"

#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "Component.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "SoundBankComponent.h"
#include "SoundEffectComponent.h"
#include "TransformComponent.h"
#include "UIButtonComponent.h"
#include "UISpriteComponent.h"
#include "UITextComponent.h"

using namespace PanicEngine;

namespace
{
    CustomComponentCallback TryMake;
    CustomComponentCallback TryGet;

    Component* AddComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;
        if (componentName == "AnimatorComponent")
        {
            newComponent = gameObject.AddComponent<AnimatorComponent>();
        }
        else if (componentName == "CameraComponent")
        {
            newComponent = gameObject.AddComponent<CameraComponent>();
        }
        else if (componentName == "FPSCameraComponent")
        {
            newComponent = gameObject.AddComponent<FPSCameraComponent>();
        }
        else if (componentName == "MeshComponent")
        {
            newComponent = gameObject.AddComponent<MeshComponent>();
        }
        else if (componentName == "ModelComponent")
        {
            newComponent = gameObject.AddComponent<ModelComponent>();
        }
        else if (componentName == "RigidBodyComponent")
        {
            newComponent = gameObject.AddComponent<RigidBodyComponent>();
        }
        else if (componentName == "SoundBankComponent")
        {
            newComponent = gameObject.AddComponent<SoundBankComponent>();
        }
        else if (componentName == "SoundEffectComponent")
        {
            newComponent = gameObject.AddComponent<SoundEffectComponent>();
        }
        else if (componentName == "TransformComponent")
        {
            newComponent = gameObject.AddComponent<TransformComponent>();
        }
        else if (componentName == "UIButtonComponent")
        {
            newComponent = gameObject.AddComponent<UIButtonComponent>();
        }
        else if (componentName == "UISpriteComponent")
        {
            newComponent = gameObject.AddComponent<UISpriteComponent>();
        }
        else if (componentName == "UITextComponent")
        {
            newComponent = gameObject.AddComponent<UITextComponent>();
        }
        else
        {
            newComponent = TryMake(componentName, gameObject);
            ASSERT(newComponent != nullptr, "GameObjectFactory: component [%s] is not valid", componentName.c_str());
        }

        return newComponent;
    }
}

Component* GetComponent(const std::string& componentName, GameObject& gameObject)
{
    Component* component = nullptr;
    if (componentName == "AnimatorComponent")
    {
        component = gameObject.GetComponent<AnimatorComponent>();
    }
    else if (componentName == "CameraComponent")
    {
        component = gameObject.GetComponent<CameraComponent>();
    }
    else if (componentName == "FPSCameraComponent")
    {
        component = gameObject.GetComponent<FPSCameraComponent>();
    }
    else if (componentName == "MeshComponent")
    {
        component = gameObject.GetComponent<MeshComponent>();
    }
    else if (componentName == "ModelComponent")
    {
        component = gameObject.GetComponent<ModelComponent>();
    }
    else if (componentName == "RigidBodyComponent")
    {
        component = gameObject.GetComponent<RigidBodyComponent>();
    }
    else if (componentName == "TransformComponent")
    {
        component = gameObject.GetComponent<TransformComponent>();
    }
    else if (componentName == "SoundBankComponent")
    {
        component = gameObject.GetComponent<SoundBankComponent>();
    }
    else if (componentName == "SoundEffectComponent")
    {
        component = gameObject.GetComponent<SoundEffectComponent>();
    }
    else if (componentName == "UIButtonComponent")
    {
        component = gameObject.GetComponent<UIButtonComponent>();
    }
    else if (componentName == "UISpriteComponent")
    {
        component = gameObject.GetComponent<UISpriteComponent>();
    }
    else if (componentName == "UITextComponent")
    {
        component = gameObject.GetComponent<UITextComponent>();
    }
    else
    {
        component = TryGet(componentName, gameObject);
        ASSERT(component != nullptr, "GameObjectFactory: component [%s] is not valid", componentName.c_str());
    }

    return component;
}

void GameObjectFactory::SetCustomMake(CustomComponentCallback callback)
{
    TryMake = callback;
}

void GameObjectFactory::SetCustomGet(CustomComponentCallback callback)
{
    TryGet = callback;
}

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& gameWorld)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");
    ASSERT(err == 0, "GameObjectFactory: failed to open file %s", templatePath.u8string().c_str());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);
    auto components = doc["Components"].GetObj();
    for (auto& component : components)
    {
        // create the component
        // add the component
        // apply the data
        Component* newComponent = AddComponent(component.name.GetString(), gameObject);
        if (newComponent != nullptr)
        {
            newComponent->Deserialize(component.value);
        }
    }
    if (doc.HasMember("Children"))
    {
        auto children = doc["Children"].GetObj();
        for (auto& child : children)
        {
            std::string name = child.name.GetString();
            std::filesystem::path childTemplate = child.value["Template"].GetString();
            GameObject* childGO = gameWorld.CreateGameObject(name, childTemplate);

            GameObjectFactory::OverrideDeserialize(child.value, *childGO);
            gameObject.AddChild(childGO);
            childGO->SetParent(&gameObject);
        }
    }
}

void GameObjectFactory::OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject)
{
    if (value.HasMember("Components"))
    {
        auto components = value["Components"].GetObj();
        for (auto& component : components)
        {
            Component* ownedComponent = GetComponent(component.name.GetString(), gameObject);
            if (ownedComponent != nullptr)
            {
                ownedComponent->Deserialize(component.value);
            }
        }
    }
}

void GameObjectFactory::SerializeGameObject(rapidjson::Document& doc, const rapidjson::Document& original, GameObject& gameObject)
{
    if (original.HasMember("Components"))
    {
        auto ogComponents = original["Components"].GetObj();
        rapidjson::Value components(rapidjson::kObjectType);
        for (auto& ogData : ogComponents)
        {
            Component* ownedComponent = GetComponent(ogData.name.GetString(), gameObject);
            if (ownedComponent != nullptr)
            {
                ownedComponent->Serialize(doc, components, ogData.value);
            }
        }
        doc.SetObject();
        doc.AddMember("Components", components, doc.GetAllocator());
    }
}
