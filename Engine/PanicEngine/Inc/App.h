#pragma once

namespace PanicEngine //In order to make it modular, put in namespace
{
    class App final //final means cannot be inherited
    {
    public:
        void Run();
        void Quit();

    private:
        bool mRunning = false;
    };
}