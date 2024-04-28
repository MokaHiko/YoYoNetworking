#pragma once

#include <Core/Memory.h>
#include <Core/Assert.h>

#include "RegistryEvents.h"
#include "ActorTypes.h"

template <typename T>
class Storage
{
public:
    virtual ~Storage<T>() = default;

    static Storage<T>& Instance()
    {
        static Storage<T>* instance = nullptr;

        if(!instance)
        {
            instance = YNEW Storage<T>();
        }

        return *instance;
    };

    template <typename... Args>
    T& Store(ActorId actor, Args &&...args)
    {
        m_actors[GetNextActorIndex()] = actor;
        m_records.insert(m_records.begin() + actor, T(std::forward<Args>(args)...));
        m_actor_count++;
        return m_records[actor];
    }

    void Remove(ActorId actor)
    {
        // TODO: Do not swap on remove last element
        std::swap(m_records[actor], m_records[m_actor_count - 1]);
        m_actor_count--;
    }

    typename std::vector<ActorId>::iterator begin() { return m_actors.begin(); }
    typename std::vector<ActorId>::iterator end() { return begin() + m_actor_count; }
private:
    uint32_t GetNextActorIndex() 
    {
        return m_actor_index++;
    }
private:
    Storage()
    {
        m_actors.resize(MAX_ACTORS);
        m_records.resize(MAX_ACTORS);

        m_actor_index = 0;
        m_actor_count = 0;
    }
private:
    friend class Registry;
    std::vector<T> m_records;

    std::vector<ActorId> m_actors;
    uint32_t m_actor_count;

    uint32_t m_actor_index;
};

class Registry
{
public:
    ~Registry() = default;

    // Singleton instance of the registry of the game
    static Registry& Instance()
    {
        static Registry* instance = nullptr;

        if(!instance)
        {
            instance = YNEW Registry();
        }

        return *instance;
    };

    ActorId CreateActor()
    {
        ActorId id = m_actor_counter++;
        m_live_actors[id] = id;

        return id;
    }

    void DestroyActor(ActorId actor)
    {
        YASSERT(actor != INVALID_ACTOR_ID, "Attempting to destroy invalid actor!");
        YASSERT(m_live_actors[actor] != INVALID_ACTOR_ID, "Attempting to destroy invalid actor!");

        m_live_actors[actor] = INVALID_ACTOR_ID;
    }

    template<typename T>
    T& Get(ActorId id)
    {
        return Storage<T>::Instance().m_records[id];
    }

    template <typename T>
    Storage<T>& GetStorage()
    {
        return Storage<T>::Instance();
    }

    template <typename T, typename... Args>
    T& Emplace(ActorId actor, Args &&...args)
    {
        Storage<T>& storage = Storage<T>::Instance();
        YASSERT(actor != INVALID_ACTOR_ID, "Invalid actor!");

        T& component = storage.Store(actor, std::forward<Args>(args)...);

        // Dispatch callbacks
        static Ref<ComponentCreatedEvent<T>> component_created_event = CreateRef<ComponentCreatedEvent<T>>(actor);
        component_created_event->actor = actor;
        yoyo::EventManager::Instance().Dispatch(component_created_event);

        return component;
    };

    template<typename T>
    void Remove(ActorId actor)
    {
        Storage<T>& storage = Storage<T>::Instance();
        storage.Remove(actor);

        // Dispatch callbacks
        static ComponentDestroyedEvent<T> component_created_event = CreateRef<ComponentDestroyedEvent<T>>(actor);
        component_created_event.actor = actor;
        yoyo::EventManager::Dispatch(component_created_event);
    };
private:
    Registry()
        :m_actor_counter(0)
    {
        m_live_actors.resize(MAX_ACTORS);
        memset(m_live_actors.data(), INVALID_ACTOR_ID, sizeof(ActorId) * MAX_ACTORS);
    };

    uint32_t m_actor_counter;
    std::vector<ActorId> m_live_actors;
};
