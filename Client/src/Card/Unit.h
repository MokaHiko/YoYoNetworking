#pragma once

#include "Actor.h"
#include "Scripting/ScriptableActor.h"

// Units are vessels that cards use to perform actions
class Unit : public ScriptableActor
{
public:
    SCRIPTABLE(Unit);

    const float GetHealth() const { return m_health; }
    void TakeDamage(float damage);

    void SetTarget(Actor target) {m_target = target;}
public:
    virtual void OnStart() override;
    virtual void OnUpdate(float dt) override;
private:
    friend class Card;
    virtual void Attack();
    virtual void Spell() {};
    virtual void Block() {};

    void Die();
private:
    Actor m_target;
    float m_health;
    float m_damage;
};

class Scene;
static class UnitFactory
{
public:
    // Creates a unit in a scene
    static Unit& CreateUnit(Ref<Scene> scene, const char* xml_path = "");
};