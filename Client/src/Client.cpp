#include "Client.h"

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

#include <Math/Math.h>

// Networking
#include "GameConsole.h" 

// Game Client
#include "Registry.h"
#include "Actor.h"

#include "Card/Card.h"
#include "Card/Unit.h"

#include "Renderable/Renderable.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneGraph.h"

ClientLayer::ClientLayer(yoyo::Application* app)
    :m_app(app), m_renderer_layer(nullptr), m_root_scene(nullptr){}

void ClientLayer::OnAttach() 
{ 
    // Instantiate game systems
    m_root_scene = CreateRef<Scene>();
}

void ClientLayer::OnDetatch() {}

void ClientLayer::OnEnable()
{
    YASSERT(m_app != nullptr, "Invalid application handle!");
    m_renderer_layer = m_app->FindLayer<yoyo::RendererLayer>();

    // ! Init systems

    // World root scene
    m_root_scene->Init();

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

    yoyo::Material::Create(default_material, "red_material")->SetVec4("diffuse_color", {1.0f, 0.0f, 0.0f, 1.0f});

    Ref<yoyo::Material> default_instanced_material = yoyo::Material::Create(default_lit_instanced, "default_instanced_material");
    Ref<yoyo::Texture> default_instanced_texture = yoyo::ResourceManager::Instance().Load<yoyo::Texture>("assets/textures/prototype_512x512_white.yo");
    default_instanced_material->SetTexture(yoyo::MaterialTextureType::MainTexture, default_instanced_texture);
    default_instanced_material->SetVec4("diffuse_color", yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    default_instanced_material->SetVec4("specular_color", yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
    default_instanced_material->SetColor(yoyo::Vec4{1.0f, 1.0f, 1.0f, 1.0f});

    yoyo::ResourceManager::Instance().Load<yoyo::Model>("assets/models/plane.yo");
    Ref<yoyo::Model> cube_model = yoyo::ResourceManager::Instance().Load<yoyo::Model>("assets/models/cube.yo");

    // Send render packet to renderer 
    yoyo::RenderPacket* rp = YNEW yoyo::RenderPacket();

	//Camera
	Ref<yoyo::Camera> camera = CreateRef<yoyo::Camera>();
    camera->SetType(yoyo::CameraType::Perspective);
	camera->position = { 0.0f, 0.0f, 10.0f };
	camera->UpdateCameraVectors();
	rp->new_camera = camera;

    // Lights
	Ref<yoyo::DirectionalLight> dir_light = CreateRef<yoyo::DirectionalLight>();
	dir_light->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dir_light->direction = yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f } *-1.0f;
	rp->new_dir_lights.push_back(dir_light);

    SendRenderPacket(rp);

    // Game
    m_render_packet = YNEW yoyo::RenderPacket(true);
    yoyo::EventManager::Instance().Subscribe(ComponentCreatedEvent<StaticMesh>::s_event_type, [&](const Ref<yoyo::Event>& event)
    {
        Actor actor(std::static_pointer_cast<ComponentCreatedEvent<StaticMesh>>(event)->actor);

        StaticMesh& mesh = actor.GetComponent<StaticMesh>();
        mesh.mesh_pass_object = CreateRef<yoyo::MeshPassObject>();
        mesh.mesh_pass_object->material = yoyo::ResourceManager::Instance().Load<yoyo::Material>("default_material");
        mesh.mesh_pass_object->mesh = yoyo::ResourceManager::Instance().Load<yoyo::StaticMesh>("Cube");

        m_render_packet->new_objects.push_back(mesh.mesh_pass_object);
        return false;
    });

    yoyo::EventManager::Instance().Subscribe(ComponentCreatedEvent<Transform>::s_event_type, [&](const Ref<yoyo::Event>& event)
    {
        return false;
    });

    yoyo::EventManager::Instance().Subscribe(ComponentCreatedEvent<Transform>::s_event_type, [&](const Ref<yoyo::Event>& event)
    {
        return false;
    });

    // Set up game
    Unit player = UnitFactory::CreateUnit(m_root_scene);
    const yoyo::Vec3& position = player.GetComponent<Transform>().GetPosition();
}

void ClientLayer::OnDisable()
{
    m_renderer_layer = nullptr;
};

void ClientLayer::OnUpdate(float dt)
{
    // Scene
    m_root_scene->Update(dt);

    // Update mesh transforms
    for(ActorId id : Registry::Instance().GetStorage<StaticMesh>())
    {
        Actor actor(id);

        // TODO: Check has component
        Transform& transform = actor.GetComponent<Transform>();
        actor.GetComponent<StaticMesh>().mesh_pass_object->model_matrix = actor.GetComponent<Transform>().GetModelMatrix();
    }

    for(ActorId id : Registry::Instance().GetStorage<Unit>())
    {
        Actor actor(id);
        Unit& unit = actor.GetComponent<Unit>();

        unit.OnUpdate(dt);
    }
    
    SendRenderPacket(m_render_packet);
}

void ClientLayer::SendRenderPacket(yoyo::RenderPacket* rp)
{
	m_renderer_layer->SendRenderPacket(rp);
}

class Client : public yoyo::Application
{
public:
    Client()
        : yoyo::Application({ "Client", 0, 0, 720, 480})
    {
        PushLayer(YNEW ClientLayer(this));
        //PushLayer(YNEW ChatLayer);
    }

    ~Client(){}
};

yoyo::Application* CreateApplication()
{
    return YNEW Client;
};
