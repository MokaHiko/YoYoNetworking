#include "UDPSocket.h"

#include "SocketUtil.h"

UDPSocket::UDPSocket(SOCKET socket)
	:m_socket(socket) {}

UDPSocket::~UDPSocket() { closesocket(m_socket); }

int UDPSocket::Bind(const SocketAddress& in_bind_address) {
	int err =
		bind(m_socket, &in_bind_address.m_socket_addr, in_bind_address.GetSize());

	if (err != 0) {
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

int UDPSocket::SendTo(const void* in_data, int length, const SocketAddress& in_to_address)
{
	int bytes_sent = sendto(m_socket, static_cast<const char*>(in_data), length, 0, &in_to_address.m_socket_addr, in_to_address.GetSize());

	if (bytes_sent >= 0)
	{
		return bytes_sent;
	}

	// ERROR
	return -SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* in_buffer, int length, SocketAddress& out_from_address)
{
	int from_length = out_from_address.GetSize();
	int read_byte_count = recvfrom(m_socket, static_cast<char*>(in_buffer), length, 0, &out_from_address.m_socket_addr, &from_length);

	if (read_byte_count >= 0)
	{
		return read_byte_count;
	}

	// Error
	return -SocketUtil::GetLastError();
}

int UDPSocket::ToggleNonBlocking(bool is_non_blocking)
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
