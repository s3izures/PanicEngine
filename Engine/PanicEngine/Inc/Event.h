#pragma once

namespace PanicEngine
{
    enum class EventType
    {
        None,
        SpacePressed,
        ShiftPressed,
    };

    class Event
    {
    public:
        Event() = default;
        Event(EventType et) : mEventType(et) {};
        virtual ~Event() = default;

        EventType GetType() const { return mEventType; }

    private:
        EventType mEventType = EventType::None;
    };

    class SpacePressedEvent : public Event
    {
    public:
        SpacePressedEvent() : Event(EventType::SpacePressed) {}
    };

    class ShiftPressedEvent : public Event
    {
    public:
        ShiftPressedEvent() : Event(EventType::ShiftPressed) {}
    };
}