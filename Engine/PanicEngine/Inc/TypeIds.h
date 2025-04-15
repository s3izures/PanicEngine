#pragma once

namespace PanicEngine
{
    enum class ComponentId
    {
        Invalid,    //Default value, not valid

        Transform,

        Count       //Last value, chain ids in custom components
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id);}\
    uint32_t GetTypeId() const override {return StaticGetTypeId();}