#include "Precompiled.h"
#include "App.h"

using namespace PanicEngine;
using namespace PanicEngine::Core;

void App::Run()
{
    (void)TimeUtil::GetTime();

    mRunning = true;
    while (mRunning)
    {
        //Update
        float time = TimeUtil::GetTime();
        LOG("RUNNING : {%.3f}", time);
        if (time > 20.0f)
        {
            Quit();
        }
    }
}

void App::Quit()
{
    mRunning = false;
}