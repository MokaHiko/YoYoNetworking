#include "Renderable.h"

#include <Renderer/RenderPass.h>
#include <Math/Quaternion.h>
#include <Math/MatrixTransform.h>

#include <Core/Assert.h>

void Transform::AddChild(Actor actor)
{
    // Remove from previous parent if exists
    Actor previous_parent = actor.GetComponent<Transform>().m_parent;
    if(previous_parent)
    {
        previous_parent.GetComponent<Transform>().RemoveChild(actor);
    }

    // Parent child self and update 
    actor.GetComponent<Transform>().m_parent = m_self;

    // Add to child list
	m_children.push_back(actor);

    // Update self before updating new child
    UpdateModelMatrix();
    actor.GetComponent<Transform>().UpdateModelMatrix();
}

void Transform::RemoveChild(Actor actor)
{
    int insert_index = -1;
    for (uint32_t i = 0; i < m_children.size(); i++)
    {
        if (m_children[i] == actor)
        {
            insert_index = i;
            break;
        }
    }

    if (insert_index < 0)
    {
        YASSERT("Child is not parented by object!");
        return;
    }

    int back_index = m_children.size() - 1;
    if (back_index > -1)
    {
        Actor back = m_children[back_index];
        if (back != actor)
        {
            m_children[insert_index] = back;
        }
    }

    actor.GetComponent<Transform>().m_parent = {}; 
}

void Transform::UpdateModelMatrix()
{
    if (m_parent)
    {
        m_model_matrix = m_parent.GetComponent<Transform>().m_model_matrix * LocalModelMatrix();
    }
    else
    {
        m_model_matrix = LocalModelMatrix();
    }

    // TODO: Clamp
    // Update euler angles
    m_rotation = yoyo::EulerAnglesFromQuat(yoyo::NormalizeQuat(m_quat_rotation));
    // if(rotation.x < 0)
    // {
    //     rotation.x = 180 - rotation.x;
    // }
    // if(rotation.y < 0)
    // {
    //     rotation.y = 180 - rotation.y;
    // }
}

const yoyo::Mat4x4& Transform::LocalModelMatrix() 
{
	if(!m_dirty)
	{
        m_local_model_matrix;
	}

    yoyo::Mat4x4 local_scale_matrix = yoyo::ScaleMat4x4(m_scale);
    yoyo::Mat4x4 local_rotation_matrix = yoyo::TransposeMat4x4(yoyo::QuatToMat4x4(m_quat_rotation));
    yoyo::Mat4x4 local_translation_matrix = yoyo::TranslationMat4x4(m_position);

    m_forward = yoyo::Normalize(local_rotation_matrix * yoyo::Vec3{0.0f, 0.0f, 1.0f});
    m_right = yoyo::Normalize(local_rotation_matrix * yoyo::Vec3{1.0f, 0.0f, 0.0f});

	m_dirty = false;
    return m_local_model_matrix = local_translation_matrix * local_rotation_matrix * local_scale_matrix;
}

void Transform::SetGlobalTransform(const yoyo::Mat4x4 transform_matrix)
{
    if(!m_parent)
    {
        m_scale = yoyo::ScaleFromMat4x4(transform_matrix);
        m_quat_rotation = yoyo::RotationFromMat4x4(transform_matrix);
        m_position = yoyo::PositionFromMat4x4(transform_matrix);

        YASSERT(0);
        return;
    }

    yoyo::Mat4x4 local_matrix = yoyo::InverseMat4x4(m_parent.GetComponent<Transform>().m_model_matrix) * transform_matrix;

	m_scale = yoyo::ScaleFromMat4x4(local_matrix);
	m_quat_rotation = yoyo::RotationFromMat4x4(local_matrix);
	m_position = yoyo::PositionFromMat4x4(local_matrix);

    UpdateModelMatrix();
}

void Transform::SetLocalTransform(const yoyo::Mat4x4 transform) 
{
	m_scale = yoyo::ScaleFromMat4x4(transform);
	m_quat_rotation = yoyo::RotationFromMat4x4(transform);
	m_position = yoyo::PositionFromMat4x4(transform);

    UpdateModelMatrix();
}

void StaticMesh::SetMaterial(Ref<yoyo::Material> material) 
{
	// TODO: Flag rebuild
	mesh_pass_object->material = material;
}
