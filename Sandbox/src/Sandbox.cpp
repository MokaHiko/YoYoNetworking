#include "Sandbox.h"

#include <Yoyo.h>

#include <Core/Log.h>

#include <Math/Math.h>
#include <Math/MatrixTransform.h>
#include <Math/Quaternion.h>
#include <Math/Random.h>

#include <Input/Input.h>

#include <Core/Memory.h>
#include <Resource/ResourceManager.h>

#include <Renderer/RendererLayer.h>
#include <Renderer/Texture.h>
#include <Renderer/Shader.h>
#include <Renderer/Material.h>
#include <Renderer/Camera.h>
#include <Renderer/Light.h>

#include <Renderer/Model.h>
#include <Renderer/Mesh.h>

SandboxLayer::SandboxLayer(yoyo::Application* app)
    :m_app(app), m_renderer_layer(nullptr) {}

SandboxLayer::~SandboxLayer() {}

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetatch() {}

void SandboxLayer::OnEnable()
{
    YASSERT(m_app != nullptr, "Invalid application handle!");
    m_renderer_layer = m_app->FindLayer<yoyo::RendererLayer>();

    // Load assets
    Ref<yoyo::Shader> default_lit = yoyo::ResourceManager::Instance().Load<yoyo::Shader>("lit_shader");
    Ref<yoyo::Shader> default_lit_instanced = yoyo::ResourceManager::Instance().Load<yoyo::Shader>("lit_instanced_shader");
    Ref<yoyo::Shader> skinned_lit = yoyo::ResourceManager::Instance().Load<yoyo::Shader>("skinned_lit_shader");

    Ref<yoyo::Material> default_material = yoyo::Material::Create(default_lit, "default_material");
    Ref<yoyo::Texture> default_texture = yoyo::ResourceManager::Instance().Load<yoyo::Texture>("assets/textures/prototype_512x512_white.yo");
    default_material->SetTexture(yoyo::MaterialTextureType::MainTexture, default_texture);
    default_material->SetVec4("diffuse_color", yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    default_material->SetVec4("specular_color", yoyo::Vec4{ 1.0, 1.0f, 1.0f, 1.0f });
    default_material->SetColor(yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

    Ref<yoyo::Material> default_instanced_material = yoyo::Material::Create(default_lit_instanced, "default_instanced_material");
    Ref<yoyo::Texture> default_instanced_texture = yoyo::ResourceManager::Instance().Load<yoyo::Texture>("assets/textures/prototype_512x512_white.yo");
    default_instanced_material->SetTexture(yoyo::MaterialTextureType::MainTexture, default_instanced_texture);
    default_instanced_material->SetVec4("diffuse_color", yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    default_instanced_material->SetVec4("specular_color", yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    default_instanced_material->SetColor(yoyo::Vec4{1.0f, 1.0f, 1.0f, 1.0f});

    yoyo::ResourceManager::Instance().Load<yoyo::Model>("assets/models/plane.yo");
    Ref<yoyo::Model> cube_model = yoyo::ResourceManager::Instance().Load<yoyo::Model>("assets/models/cube.yo");

	//Camera
	Ref<yoyo::Camera> camera = CreateRef<yoyo::Camera>();
	camera->position = { 0.0f, 0.0f, 10.0f };
	camera->UpdateCameraVectors();

    // Lights
	Ref<yoyo::DirectionalLight> dir_light = CreateRef<yoyo::DirectionalLight>();
	dir_light->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dir_light->direction = yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f } *-1.0f;

    // Scene objects
	Ref<yoyo::MeshPassObject> cube = CreateRef<yoyo::MeshPassObject>();
	cube->mesh = yoyo::ResourceManager::Instance().Load<yoyo::StaticMesh>("Cube");
	cube->material = yoyo::ResourceManager::Instance().Load<yoyo::Material>("default_material");
	cube->model_matrix = {};

    // Send render packet to renderer 
    yoyo::RenderPacket* rp = YNEW yoyo::RenderPacket();
	rp->new_camera = camera;
	rp->new_dir_lights.push_back(dir_light);
	rp->new_objects.push_back(cube);

    SendRenderPacket(rp);
}

void SandboxLayer::OnDisable()
{
    m_renderer_layer = nullptr;
};

void SandboxLayer::OnUpdate(float dt)
{
}

void SandboxLayer::SendRenderPacket(yoyo::RenderPacket* rp)
{
	m_renderer_layer->SendRenderPacket(rp);
}

class Sandbox : public yoyo::Application
{
public:
    Sandbox()
        : yoyo::Application({ "Sandbox", 0, 0, 1920, 1080 })
    {
        PushLayer(YNEW SandboxLayer(this));
    }

    ~Sandbox(){}
};

yoyo::Application* CreateApplication()
{
    return YNEW Sandbox;
};