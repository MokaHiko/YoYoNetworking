#include "SceneGraph.h"

void SceneGraph::Shutdown()
{
	// Unsubscribe
}

void SceneGraph::Update(float dt)
{
	YASSERT(m_root, "Invalid root!");
	RecursiveUpdate(m_root.GetComponent<Transform>());
}

// void SceneGraph::OnComponentCreated(Entity e, TransformComponent* transform)
// {
// 	// Define self
// 	e.GetComponent<TransformComponent>().self = e;
// }

// void SceneGraph::OnComponentDestroyed(Entity e, TransformComponent* transform)
// {
// 	if (auto parent = e.GetComponent<TransformComponent>().parent)
// 	{
// 		parent.GetComponent<TransformComponent>().RemoveChild(e);
// 	}

// 	// Destroy children immediately
// 	for (uint32_t i = 0; i < e.GetComponent<TransformComponent>().children_count; i++)
// 	{
// 		GetScene()->Destroy(e.GetComponent<TransformComponent>().children[i]);
// 	}

// 	e.GetComponent<TransformComponent>().self = {};
// }

void SceneGraph::Init(Actor root) 
{
	m_root = root;
}

void SceneGraph::RecursiveUpdate(Transform &node) {
        for (Actor child : node.GetChildren())
	{
		Transform& transform = child.GetComponent<Transform>();

		transform.UpdateModelMatrix();
		RecursiveUpdate(transform);
	}
}

// void SceneGraph::RecursiveUpdate(Entity e)
// {
// 	for (uint32_t i = 0; i < e.GetComponent<TransformComponent>().children_count; i++)
// 	{
// 		TransformComponent& transform = e.GetComponent<TransformComponent>().children[i].GetComponent<TransformComponent>();

// 		// TODO: Dirty Check
// 		transform.UpdateModelMatrix();
// 		RecursiveUpdate(e.GetComponent<TransformComponent>().children[i]);
// 	}
// }