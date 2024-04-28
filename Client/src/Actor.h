#pragma once

#include "ActorTypes.h"
#include "Registry.h"

/*
    Actors are...

    Components are structures that can be linked to an actor. Components are required to provide public default constructors and destructors.
*/
class Actor
{
public:
    Actor() = default;
    ~Actor() = default;

    Actor(ActorId id)
        :m_id(id) {}

    Actor GameObject()
    {
        return *this;
    }

    template<typename T>
    T& GetComponent()
    {
        YASSERT(m_id != INVALID_ACTOR_ID);
        return Registry::Instance().Get<T>(m_id);
    }

    template <typename T, typename... Args>
    T& AddComponent(Args &&...args)
    {
        YASSERT(m_id != INVALID_ACTOR_ID);
        return Registry::Instance().Emplace<T>(m_id, std::forward<Args>(args)...);
    }

    operator bool() const{return m_id != INVALID_ACTOR_ID;}
    operator ActorId() const { return m_id; }

    friend bool operator==(Actor a1, Actor a2){return a1.m_id == a2.m_id;}
    friend bool operator!=(Actor a1, Actor a2){return a1.m_id != a2.m_id;}
private:
    ActorId m_id = INVALID_ACTOR_ID;
};