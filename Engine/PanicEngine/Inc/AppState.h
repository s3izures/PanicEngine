#pragma once

namespace PanicEngine
{
    class AppState
    {
    public:
        virtual ~AppState() = default;
        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltatime) {}
        virtual void Render() {}
        virtual void DebugUI(){}
    };
}