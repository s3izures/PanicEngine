#include <PanicEngine/Inc/PanicEngine.h>

class GameState : public PanicEngine::AppState {
public:
    void Initialize() override
    {
        LOG("Game State Initialized");
    }
    void Terminate() override
    {
        LOG("Game State Terminated");
    }
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    PanicEngine::AppConfig config;
    config.appName = L"Hello Window";

    PanicEngine::App& myApp = PanicEngine::MainApp();
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);

    return(0);
}