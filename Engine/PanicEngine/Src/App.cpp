#include "Precompiled.h"
#include "App.h"

using namespace PanicEngine;
using namespace PanicEngine::Core;

void App::Run()
{
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        L"Hello Window",
        600,
        400
    );

    (void)TimeUtil::GetTime();

    mRunning = true;

    while (mRunning)
    {
        //Update
        myWindow.ProcessMessage();
        if (!myWindow.IsActive())
        {
            Quit();
        }
    }

    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}