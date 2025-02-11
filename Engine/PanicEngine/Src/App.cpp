#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace PanicEngine;
using namespace PanicEngine::Core;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Input;
using namespace PanicEngine::Physics;

void App::Run(const AppConfig& config)
{
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.appName,
        config.winWidth,
        config.winHeight
    );

    ASSERT(myWindow.IsActive(), "App: failed to create window");

    //Init singletons
    auto handle = myWindow.GetWindowHandle();
    GraphicsSystem::StaticInitialize(handle, false);
    InputSystem::StaticInitialize(handle);
    DebugUI::StaticInitialize(handle, false, true);
    SimpleDraw::StaticInitialize(config.maxDrawLines);
    TextureCache::StaticInitialize("../../Assets/Images");
    ModelCache::StaticInitialize();

    PhysicsWorld::Settings settings;
    PhysicsWorld::StaticInitialize(settings);

    ASSERT(mCurrentState != nullptr, "App: no current state available");
    mCurrentState->Initialize();

    InputSystem* input = InputSystem::Get();
    mRunning = true;
    while (mRunning)
    {
        myWindow.ProcessMessage();
        input->Update();

        if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
            break;
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        float deltaTime = TimeUtil::GetDeltaTime();

#ifdef _DEBUG
        if (deltaTime < 0.5f)
#endif
        {
            mCurrentState->Update(deltaTime);
            PhysicsWorld::Get()->Update(deltaTime);
        }


        GraphicsSystem* gs = GraphicsSystem::Get();
        gs->BeginRender();
            mCurrentState->Render();
            DebugUI::BeginRender();
                mCurrentState->DebugUI();
            DebugUI::EndRender();
        gs->EndRender();


    }
    mCurrentState->Terminate(); //FILO, First in Last Out
    //End state
    PhysicsWorld::StaticTerminate();
    ModelCache::StaticTerminate();
    TextureCache::StaticTerminate();
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    InputSystem::StaticTerminate();
    GraphicsSystem::StaticTerminate();
    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}

void PanicEngine::App::ChangeState(const std::string& stateName)
{
    auto iter = mAppStates.find(stateName);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}
