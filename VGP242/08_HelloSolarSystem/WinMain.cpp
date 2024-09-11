#include <PanicEngine/Inc/PanicEngine.h>
#include "GameState.h"

using namespace PanicEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Solar System";
    config.winWidth = 800;
    config.winHeight = 800;

    App& myApp = MainApp();

    myApp.AddState<GameState>("GameState");

    myApp.Run(config);
    return(0);
}

/*Create a solar system
- use a skybox sphere to create a space background
- add a sun in the center
- 9 planets (yes 9) rotating around the sun (year)
- have each planet rotate independently (day)
- Use simple draw to draw the rings of the planet orbits
- Use ImGui to toggle the rings on/off
- Use ImGui to adjust the speed of the rotations of the planets
- Have a render target in ImGui that lets you focus on a desired planet (use a combo box to select a planet to display)*/