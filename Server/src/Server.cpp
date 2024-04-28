#include "Server.h"

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

// Move to network layer
#include <FumblyNet.h>

ServerLayer::ServerLayer(yoyo::Application* app)
    :m_app(app), m_renderer_layer(nullptr) {}

ServerLayer::~ServerLayer() {}


#define SERVER_ADDRESS "127.0.0.1:6000"
static TCPSocketPtr listen_socket;
static SocketAddressPtr server_address;

static std::vector<TCPSocketPtr> read_block_sockets;
static std::vector<TCPSocketPtr> readable_sockets;

void ServerLayer::OnAttach() 
{
    // TODO: Networking layer
    if(SocketUtil::StartUp() != 0)
    {
        YERROR("Failed to start server!");
    }

	listen_socket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
    listen_socket->ToggleNonBlocking(true);
	server_address = SocketAddressFactory::CreateIPv4FromString(SERVER_ADDRESS);

    // Bind server listenign socket
    if(listen_socket->Bind(*server_address) == 0)
    {
        YINFO("Server started on: %s", SERVER_ADDRESS);
        read_block_sockets.push_back(listen_socket);
        listen_socket->Listen();
    }
    else
    {
        YINFO("Failed to bind listening socket");
        exit(-1);
    }
}

void MessageAll(char* message, size_t message_size, TCPSocketPtr except = nullptr)
{
    for(auto& socket : read_block_sockets)
    {
        if(socket == listen_socket)
        {
            continue;
        }

		if(socket == except)
		{
			continue;
		}

        if (socket->Send(message, message_size) < 0)
        {
            YERROR("Failed to send messages back to users!");
        }
    }
}


void RunServer()
{
    // Max time allocated to server
    long interval = 1 / 1000.0f; 

    // Check if any messages are readable
    if(SocketUtil::Select(interval, &read_block_sockets, &readable_sockets, nullptr, nullptr, nullptr, nullptr))
    {
        for(const TCPSocketPtr& socket : readable_sockets)
        {
            if(socket == listen_socket)
            {
                YINFO("New connection!");
                SocketAddress client_address;
                TCPSocketPtr new_socket = listen_socket->Accept(client_address);
                new_socket->ToggleNonBlocking(true);
                read_block_sockets.push_back(new_socket);
                YINFO("Acccepted client:!");
            }
            else
            {
                static char message_buffer[1024];
                memset(message_buffer, 0, sizeof(1024));
                int bytes_recieved = socket->Receive(message_buffer, sizeof(1024));
                if(bytes_recieved > 0)
                {
                    YINFO("Message All From[%d]: %s", socket, message_buffer);
                    MessageAll(message_buffer, bytes_recieved, socket);
                }
            }

            readable_sockets.clear();
        }
    }
}

void ServerLayer::OnDetatch() {}

void ServerLayer::OnEnable()
{
    YASSERT(m_app != nullptr, "Invalid application handle!");
    m_renderer_layer = m_app->FindLayer<yoyo::RendererLayer>();

	//Camera
	Ref<yoyo::Camera> camera = CreateRef<yoyo::Camera>();
	camera->position = { 0.0f, 0.0f, 10.0f };
	camera->UpdateCameraVectors();

    // Lights
	Ref<yoyo::DirectionalLight> dir_light = CreateRef<yoyo::DirectionalLight>();
	dir_light->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dir_light->direction = yoyo::Vec4{ 1.0f, 1.0f, 1.0f, 1.0f } *-1.0f;

    // Send render packet to renderer 
    yoyo::RenderPacket* rp = YNEW yoyo::RenderPacket();
	rp->new_camera = camera;
	rp->new_dir_lights.push_back(dir_light);

    SendRenderPacket(rp);
}

void ServerLayer::OnDisable()
{
    SocketUtil::Shutdown();

    m_renderer_layer = nullptr;
};

void ServerLayer::OnUpdate(float dt)
{
    RunServer();
}

void ServerLayer::SendRenderPacket(yoyo::RenderPacket* rp)
{
	m_renderer_layer->SendRenderPacket(rp);
}

class Server : public yoyo::Application
{
public:
    Server()
        : yoyo::Application({ "Server", 0, 0, 720, 480})
    {
        PushLayer(YNEW ServerLayer(this));
    }

    ~Server(){}
};

yoyo::Application* CreateApplication()
{
    return YNEW Server;
};