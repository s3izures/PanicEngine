#include <PanicEngine/Inc/PanicEngine.h>
#include "GameState.h"

using namespace PanicEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Render Target";
    config.winWidth = 800;
    config.winHeight = 800;

    App& myApp = MainApp();

    myApp.AddState<GameState>("GameState");

    myApp.Run(config);
    return(0);
}