#pragma once

#include <Core/Memory.h>

#include <Math/MathTypes.h>
#include <Math/Quaternion.h>
#include <Actor.h>

// Forward declaration
namespace yoyo {class IMesh;}
namespace yoyo {class Material;}
namespace yoyo {class MeshPassObject;}

struct Tag
{
    std::string name;
};

struct Transform
{
public:
    inline const yoyo::Vec3& GetPosition() const {return m_position;}
    inline const yoyo::Vec3& GetScale() const {return m_scale ;}
    inline const yoyo::Quat& GetRotation() const {return m_quat_rotation;}

    inline const yoyo::Mat4x4& GetModelMatrix() const {return m_model_matrix;}

    inline const yoyo::Vec3 GetGlobalPosition() const {return {m_model_matrix.data[12],m_model_matrix.data[13],m_model_matrix.data[14]};}

    void SetPosition(const yoyo::Vec3& position) {m_position = position; m_dirty = true;}
    void SetScale(const yoyo::Vec3& scale ) {m_scale = scale; m_dirty = true;}
    void SetRotation(const yoyo::Quat& rotation) {m_quat_rotation = rotation; m_dirty = true;}

    void SetGlobalTransform(const yoyo::Mat4x4 transform);
    void SetLocalTransform(const yoyo::Mat4x4 transform);

    // Returns the forward component of the transform
    const yoyo::Vec3& Forward() const {return m_forward;}; 
    const yoyo::Vec3& Right () const {return m_right;}; 

    Actor GetSelf() const {return m_self;}
    Actor GetParent() const {return m_parent;}
    const std::vector<Actor>& GetChildren() const {return m_children;}

    // Updates the model matrix of the current transform
    void UpdateModelMatrix();
public:
    void AddChild(Actor actor);
    void RemoveChild(Actor actor);
private:
    const yoyo::Mat4x4& LocalModelMatrix();

    yoyo::Vec3 m_position{ 0.0f , 0.0f, 0.0f};
    yoyo::Vec3 m_rotation{ 0.0f , 0.0f, 0.0f}; // Rotation in euler radians
    yoyo::Vec3 m_scale { 1.0f , 1.0f, 1.0f};

    yoyo::Quat m_quat_rotation = {0.0f, 0.0f, 0.0f, 1.0f};
    yoyo::Mat4x4 m_model_matrix{};
    yoyo::Mat4x4 m_local_model_matrix{};

    Actor m_self = {};
    Actor m_parent = {};

    std::vector<Actor> m_children = {};

    yoyo::Vec3 m_forward = { 0.0f, 0.0f, 1.0f };
    yoyo::Vec3 m_right = { 1.0f, 0.0f, 0.0f };
    bool m_dirty = false;
};

struct StaticMesh
{
    StaticMesh() = default;
    ~StaticMesh() = default;

    void SetMaterial(Ref<yoyo::Material> material);

    Ref<yoyo::MeshPassObject> mesh_pass_object;
};

// struct SkeletalMesh
// {
// public:
//     SkeletalMesh() = default;
//     ~SkeletalMesh()= default;

//     void SetMesh(Ref<yoyo::IMesh> mesh);
//     void SetMaterial(Ref<yoyo::Material> material);
// private:
//     Ref<yoyo::MeshPassObject> mesh_pass_object;
// };