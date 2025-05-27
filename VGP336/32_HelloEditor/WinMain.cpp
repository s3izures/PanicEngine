#include <PanicEngine/Inc/PanicEngine.h>
#include "GameState.h"
#include "EditTemplateState.h"

using namespace PanicEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Editor";
    config.winWidth = 800;
    config.winHeight = 800;

    App& myApp = MainApp();

    myApp.AddState<GameState>("GameState");
    myApp.AddState<EditTemplateState>("EditTemplateState");

    myApp.Run(config);
    return(0);
}