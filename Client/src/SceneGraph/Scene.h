#pragma once


#include <Math/Math.h>

#include "Actor.h"
#include "SceneGraph.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void Init();
    void Update(float dt);

    // Insantiates an actor in the scene
    Actor Instantiate(const yoyo::Vec3& position = {0.0f, 0.0f, 0.0f}, const yoyo::Quat quat = {0.0f, 0.0f, 0.0f, 1.0f});
private:
    SceneGraph m_scene_graph;
    std::vector<Ref<Scene>> m_children;
};
