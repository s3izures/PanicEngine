#include "EditTemplateState.h"
#include "CustomDebugDrawService.h"
#include "CustomDebugDrawComponent.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Core;
using namespace PanicEngine::Input;
using namespace PanicEngine::Audio;
using namespace PanicEngine::Physics;

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

void EditTemplateState::Initialize()
{
    //adds delegate callback to create a custom service
    GameWorld::SetCustomService(CustomServiceMake);
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    GameObjectFactory::SetCustomGet(CustomComponentGet);

    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/editor_level.json", true);
    PhysicsService* ps = mGameWorld.GetService<PhysicsService>();
    ps->SetEnabled(false);
}

void EditTemplateState::Terminate()
{
    mGameWorld.EditTemplate("");
    mGameWorld.Terminate();
}

void EditTemplateState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void EditTemplateState::Render()
{
    mGameWorld.Render();
}

void EditTemplateState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    if (ImGui::Button("Exit"))
    {
        MainApp().ChangeState("GameState");
    }
    ImGui::End();
}
