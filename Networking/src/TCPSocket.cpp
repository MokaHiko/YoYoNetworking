#include "TCPSocket.h"
#include "SocketUtil.h"

TCPSocket::TCPSocket(SOCKET in_socket)
	:m_socket(in_socket){}

TCPSocket::~TCPSocket()
{
	closesocket(m_socket);
}

int TCPSocket::Connect(const SocketAddress &in_address) 
{
	int err = connect(m_socket, &in_address.m_socket_addr, in_address.GetSize());

	if(err < 0)
	{
		return -SocketUtil::GetLastError();
	}

	return NO_ERROR; 
}

int TCPSocket::Bind(const SocketAddress &in_address) 
{ 
	int err = bind(m_socket, &in_address.m_socket_addr, in_address.GetSize());

	if(err < 0)
	{
		return -SocketUtil::GetLastError();
	}

	return NO_ERROR; 
}

int TCPSocket::Listen(int in_back_log) 
{ 
	int err = listen(m_socket, in_back_log);

	if(err < 0)
	{
		return -SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress &in_from_address) 
{
	int address_length = in_from_address.GetSize();
	SOCKET connection_socket = accept(m_socket, &in_from_address.m_socket_addr, &address_length);

	if(connection_socket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(connection_socket));
	}

	// TODO: Print
	SocketUtil::GetLastError();
	return nullptr;
}

int TCPSocket::Send(const void *in_data, int length) 
{ 
	int bytes_sent = send(m_socket, static_cast<const char*>(in_data), length , 0);

	if(bytes_sent < 0)
	{
		return -SocketUtil::GetLastError();
	}

	return bytes_sent;
}

int TCPSocket::Receive(void *in_data, int length) 
{ 
	int bytes_received = recv(m_socket, static_cast<char*>(in_data), length, 0);

	if(bytes_received < 0)
	{
		return -SocketUtil::GetLastError();
	}

	return bytes_received;
}

int TCPSocket::ToggleNonBlocking(bool is_non_blocking)
{
	// TODO: assert socket exists

#ifdef _WIN32
	u_long arg = is_non_blocking ? 1 : 0;
	int result = ioctlsocket(m_socket, FIONBIO, &arg);
#else
	// TODO: No Impl
#endif
	if(result == SOCKET_ERROR)
	{
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

