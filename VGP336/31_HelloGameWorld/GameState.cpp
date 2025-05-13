#include "GameState.h"
#include "CustomDebugDrawService.h"
#include "CustomDebugDrawComponent.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;

Service* CustomServiceMake(const std::string& serviceName, GameWorld& gameWorld)
{
    if (serviceName == "CustomDebugDrawService")
    {
        return gameWorld.AddService<CustomDebugDrawService>();
    }
    return nullptr;
}

Component* CustomComponentMake(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.AddComponent<CustomDebugDrawComponent>();
    }
    return nullptr;
}

Component* CustomComponentGet(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.GetComponent<CustomDebugDrawComponent>();
    }
    return nullptr;
}

void GameState::Initialize()
{
    //adds delegate callback to create a custom service
    GameWorld::SetCustomService(CustomServiceMake);
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/test_level.json");
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
