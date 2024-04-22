#include "GameConsole.h"

#include <Math/Math.h>

#include <ImGui/ImGuiLayer.h>
#include <FumblyNet.h>

#define SERVER_ADDRESS "127.0.0.1:6000"
static TCPSocketPtr client_socket;
static SocketAddressPtr server_address;

void ChatLayer::OnEnable() 
{
	// TODO: Put in networking layer
	if (SocketUtil::StartUp() != 0)
	{
		YERROR("Failed to start up networking layer!");
	}

	server_address = SocketAddressFactory::CreateIPv4FromString(SERVER_ADDRESS);

	client_socket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
	auto client_address = SocketAddress(INADDR_ANY, 60001);
	if (client_socket->Bind(client_address) != 0)
	{
		YERROR("Failed to bind client socket to address!");
	}
	// Wait to connect to server
	int err;
	if (err = client_socket->Connect(*server_address) == 0)
	{
		YINFO("Connection to server established!");
		client_socket->ToggleNonBlocking(true);
	}
	else
	{
		YINFO("Failed to connecto server: [Error]: %d", err);
		exit(-1);
	}

	// Get context from imgui layer
	ImGuiContext* ctx = yoyo::ImGuiLayer::GetContext();
	YASSERT(ctx != nullptr, "Invalid ImGuiContext!");
	ImGui::SetCurrentContext(ctx);
	ImGuizmo::SetImGuiContext(ctx);
}

void ChatLayer::OnDisable() 
{
	SocketUtil::Shutdown();
}

void ChatLayer::OnUpdate(float dt)
{ 
	static char data[1024];
	memset(data, 0, 1024);
	if (client_socket->Receive(data, 1024) > 0)
	{
		// TODO: Cmd vs Chat Log
		m_game_console.AddLog("user: %s", data);
	}
}

void ChatLayer::OnDetatch() {}

void ChatLayer::OnImGuiRender()
{
	static bool open = true;
	m_game_console.Draw("Game Console", &open);
}

void GameConsole::SendChatMessage(const char* message, size_t message_size)
{
	YINFO("SENDING MESSAGE!");
	YASSERT(client_socket, "Cannot send message to invalid socket!");
	if(client_socket->Send(message, message_size) < 0)
	{
		YERROR("Failed to send message: %s", message);
	}
}
