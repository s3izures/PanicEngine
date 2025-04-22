#pragma once

namespace PanicEngine
{
    enum class ComponentId
    {
        Invalid,    //----- Default value, not valid
        Transform,  //Transform component for location
        Camera,     //Contains camera for view
        FPSCamera,  //FPS camera
        Mesh,       //Adds mesh shape
        Count       //----- Last value, chain ids in custom components
    };

    enum class ServiceId
    {
        Invalid,    //----- Default value, not valid
        Camera,     //Camera manager
        Render,     //Render object in world
        Count       //----- Last value, chain ids in custom components
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id);}\
    uint32_t GetTypeId() const override {return StaticGetTypeId();}