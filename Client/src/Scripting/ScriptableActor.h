#pragma once

#include "Actor.h"
#include "Platform/Platform.h"

typedef uint64_t ScriptId;

#define SCRIPTABLE(script_type) script_type(ActorId id = INVALID_ACTOR_ID) : ScriptableActor(id) {}\
                                const ScriptId ScriptType() const override {return s_script_type;}\
                                inline static const ScriptId s_script_type = yoyo::Platform::GenerateUUIDV4();\

class IScript
{
public:
    IScript() = default;
    virtual ~IScript() = default;

    virtual const ScriptId ScriptType() const = 0;

    virtual void OnStart() {};
    virtual void OnUpdate(float dt) {};
};

// Base class for scripting components
class ScriptableActor : public Actor, public IScript
{
public:
    ScriptableActor() {};
    virtual ~ScriptableActor() = default;

    ScriptableActor(ActorId id = INVALID_ACTOR_ID);
};