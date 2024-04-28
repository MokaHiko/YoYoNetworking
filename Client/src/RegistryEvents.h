#pragma once

#include <Events/Event.h>

#include "ActorTypes.h"

template<typename T>
class ComponentCreatedEvent : public yoyo::Event
{
public:
    EVENT_TYPE(ComponentCreatedEvent)

    ComponentCreatedEvent(ActorId _actor)
        :actor(_actor){}
    virtual ~ComponentCreatedEvent() = default;

    ActorId actor;
};

template<typename T>
class ComponentDestroyedEvent : public yoyo::Event
{
public:
    EVENT_TYPE(ComponentDestroyedEvent)

    ComponentDestroyedEvent(ActorId _actor)
        :actor(_actor){}
    virtual ~ComponentDestroyedEvent() = default;

    ActorId actor;
};
