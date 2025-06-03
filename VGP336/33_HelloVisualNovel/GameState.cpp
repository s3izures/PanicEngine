#include "GameState.h"
#include "CustomDialogueComponent.h"
#include "CustomDialogueService.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

namespace
{
    Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
    {
        if (serviceName == "CustomDialogueService")
        {
            return gameWorld.AddService<CustomDialogueService>();
        }
        return nullptr;
    }

    Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
    {
        if (componentName == "CustomDialogueComponent")
        {
            return gameObject.AddComponent<CustomDialogueComponent>();
        }
        return nullptr;
    }

    Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
    {
        if (componentName == "CustomDialogueComponent")
        {
            return gameObject.GetComponent<CustomDialogueComponent>();
        }
        return nullptr;
    }
}

void GameState::Initialize()
{
    //adds delegate callback to create a custom service
    GameWorld::SetCustomService(CustomServiceMake);
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/visual_novel.json");
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
    
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();
}
