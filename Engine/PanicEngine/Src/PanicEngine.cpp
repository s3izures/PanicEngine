#include "AppState.h"
#include "Precompiled.h"
#include "PanicEngine.h"

PanicEngine::App& PanicEngine::MainApp()
{
    static PanicEngine::App sApp;
    return sApp;
}