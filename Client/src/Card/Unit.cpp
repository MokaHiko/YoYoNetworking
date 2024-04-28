#include "Unit.h"

#include <Input/Input.h>
#include "SceneGraph/Scene.h"

#include "Renderable/Renderable.h"
#include "Resource/ResourceManager.h"

void Unit::TakeDamage(float damage)
{
	GetComponent<StaticMesh>().SetMaterial(yoyo::ResourceManager::Instance().Load<yoyo::Material>("red_material"));
	m_health -= damage;
	if (m_health <= 0)
	{
		Die();
	}
}

void Unit::Die() 
{
}

void Unit::OnStart()
{
	m_health = 100;
}

void Unit::OnUpdate(float dt)
{
	float ms = 10.0f;
	yoyo::Vec3 direction = {0.0f};

	if(yoyo::Input::GetKey(yoyo::KeyCode::Key_a))
	{
		direction.x -= 1;
	}

	if(yoyo::Input::GetKey(yoyo::KeyCode::Key_d))
	{
		direction.x += 1;
	}

	if(yoyo::Input::GetKey(yoyo::KeyCode::Key_w))
	{
		direction.y += 1;
	}

	if(yoyo::Input::GetKey(yoyo::KeyCode::Key_s))
	{
		direction.y -= 1;
	}

	const yoyo::Vec3 position = GetComponent<Transform>().GetPosition();
	direction = Normalize(direction);
	GetComponent<Transform>().SetPosition(position + (direction * ms * dt));
}

void Unit::Attack() 
{
	if(m_target)
	{
		m_target.GetComponent<Unit>().TakeDamage(10);
	}
}

Unit& UnitFactory::CreateUnit(Ref<Scene> scene, const char* xml_path)
{
	Actor actor = scene->Instantiate();
	actor.AddComponent<Transform>();
	actor.AddComponent<Tag>();

	actor.AddComponent<StaticMesh>(); 
	return actor.AddComponent<Unit>(actor);
}
