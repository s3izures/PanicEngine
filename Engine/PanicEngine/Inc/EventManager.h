#pragma once
#include "Event.h"

namespace PanicEngine
{
    using ListenerId = std::size_t;
    using EventCallback = std::function<void(const Event&)>;

    class EventManager final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static EventManager* Get();
        static void Broadcast(const Event& e);

        EventManager() = default;
        ~EventManager();

        void Initialize();
        void Terminate();

        ListenerId AddListener(EventType eventType, const EventCallback cb);
        void RemoveListener(EventType eventType, ListenerId listenerId);

    private:
        void BroadcastPrivate(const Event& e);

        using EventListeners = std::unordered_map<EventType, std::unordered_map<ListenerId, EventCallback>>;
        EventListeners mEventListeners;
        ListenerId mNextListenerId = 0;
    };
}