#pragma once

#include "Renderable/Renderable.h"
#include "Actor.h"

class SceneGraph
{
public:
    SceneGraph() = default;
    virtual ~SceneGraph() = default;

    virtual void Init(Actor root);
    virtual void Shutdown();
    virtual void Update(float dt);

    Actor GetRoot() const {return m_root;}

    // virtual void OnComponentCreated(Entity e, TransformComponent* component);
    // virtual void OnComponentDestroyed(Entity e, TransformComponent* component);
private:
    void RecursiveUpdate(Transform& node);
    Actor m_root;
};
