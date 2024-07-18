#include <PanicEngine/Inc/PanicEngine.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    PanicEngine::App myApp;
    myApp.Run();

    return(0);
}