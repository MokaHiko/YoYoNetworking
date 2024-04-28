#include "Scene.h"
#include "Registry.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init() 
{
	Actor root = Registry::Instance().CreateActor();
	root.AddComponent<Transform>();
	m_scene_graph.Init(root);
}

void Scene::Update(float dt) 
{
	// Update scene graph
	m_scene_graph.Update(dt);

	for(Ref<Scene>& child_scene : m_children)
	{
		child_scene->Update(dt);
	}
}

Actor Scene::Instantiate(const yoyo::Vec3& position, const yoyo::Quat quat)
{
	Actor actor = Registry::Instance().CreateActor();
	actor.AddComponent<Tag>();

	Transform& transform = actor.AddComponent<Transform>();
	transform.SetPosition(position);
	transform.SetRotation(quat);

	// Parent to scene root
	Actor root = m_scene_graph.GetRoot();
	root.GetComponent<Transform>().AddChild(actor);

	return actor;
}
