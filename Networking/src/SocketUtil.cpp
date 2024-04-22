#include "SocketUtil.h"

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}

int SocketUtil::StartUp() 
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		assert(0 && "Failed to initialize winsock!");
		return -1;
	}
}

void SocketUtil::Shutdown() 
{
    WSACleanup();
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily in_family)
{
	SOCKET udp_socket = socket((int)in_family, SOCK_DGRAM, IPPROTO_UDP);

	if (udp_socket != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(udp_socket));
	}

	SocketUtil::GetLastError();
	return nullptr;
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily in_family)
{
	SOCKET udp_socket = socket((int)in_family, SOCK_STREAM, IPPROTO_TCP);

	if (udp_socket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(udp_socket));
	}

	SocketUtil::GetLastError();
	return nullptr;
}

fd_set* SocketUtil::FillSetFromVector(fd_set& out_set, const std::vector<TCPSocketPtr>* in_sockets)
{
	if (in_sockets)
	{
		FD_ZERO(&out_set);
		for (const TCPSocketPtr& socket : *in_sockets)
		{
			FD_SET(socket->m_socket, &out_set);
		};
		return &out_set;
	}

	return nullptr;
}

fd_set* SocketUtil::FillSetFromVector(fd_set& out_set, const std::vector<UDPSocketPtr>* in_sockets)
{
	if (in_sockets)
	{
		FD_ZERO(&out_set);
		for (const UDPSocketPtr& socket : *in_sockets)
		{
			FD_SET(socket->m_socket, &out_set);
		};
		return &out_set;
	}

	return nullptr;
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* out_sockets, const std::vector<TCPSocketPtr>* in_sockets, const fd_set& in_set)
{
	if (out_sockets && in_sockets)
	{
		out_sockets->clear();
		for (const TCPSocketPtr& socket : *in_sockets)
		{
			if (FD_ISSET(socket->m_socket, &in_set))
			{
				out_sockets->push_back(socket);
			}
		}
	}
}

void SocketUtil::FillVectorFromSet(std::vector<UDPSocketPtr>* out_sockets, const std::vector<UDPSocketPtr>* in_sockets, const fd_set& in_set)
{
	out_sockets->clear();
	if (out_sockets && in_sockets)
	{
		for (const UDPSocketPtr& socket : *in_sockets)
		{
			if (FD_ISSET(in_sockets, &in_set))
			{
				out_sockets->push_back(socket);
			}
		}
	}
}

int SocketUtil::Select(const std::vector<TCPSocketPtr>* in_read_set, std::vector<TCPSocketPtr>* out_read_set, const std::vector<TCPSocketPtr>* in_write_set, std::vector<TCPSocketPtr>* out_write_set, const std::vector<TCPSocketPtr>* in_except_set, std::vector<TCPSocketPtr>* out_except_set)
{
	fd_set read, write, except;

	fd_set* read_ptr = FillSetFromVector(read, in_read_set);
	fd_set* write_ptr = FillSetFromVector(write, in_write_set);
	fd_set* except_ptr = FillSetFromVector(except, in_except_set);

	int count = select(0, read_ptr, write_ptr, except_ptr, nullptr);

	if (count > 0)
	{
		FillVectorFromSet(out_read_set, in_read_set, read);
		FillVectorFromSet(out_write_set, in_write_set, write);
		FillVectorFromSet(out_except_set, in_except_set, except);
	}

	return count;
}

int SocketUtil::Select(long interval, const std::vector<TCPSocketPtr>* in_read_set, std::vector<TCPSocketPtr>* out_read_set, const std::vector<TCPSocketPtr>* in_write_set, std::vector<TCPSocketPtr>* out_write_set, const std::vector<TCPSocketPtr>* in_except_set, std::vector<TCPSocketPtr>* out_except_set)
{
	fd_set read, write, except;

	fd_set* read_ptr = FillSetFromVector(read, in_read_set);
	fd_set* write_ptr = FillSetFromVector(write, in_write_set);
	fd_set* except_ptr = FillSetFromVector(except, in_except_set);

	timeval time_interval = {};
	time_interval.tv_sec = interval;
	int count = select(0, read_ptr, write_ptr, except_ptr, &time_interval);

	if (count > 0)
	{
		FillVectorFromSet(out_read_set, in_read_set, read);
		FillVectorFromSet(out_write_set, in_write_set, write);
		FillVectorFromSet(out_except_set, in_except_set, except);
	}

	return count;

}

int SocketUtil::Select(const std::vector<UDPSocketPtr>* in_read_set, std::vector<UDPSocketPtr>* out_read_set, const std::vector<UDPSocketPtr>* in_write_set, std::vector<UDPSocketPtr>* out_write_set, const std::vector<UDPSocketPtr>* in_except_set, std::vector<UDPSocketPtr>* out_except_set)
{
	fd_set read, write, except;

	fd_set* read_ptr = FillSetFromVector(read, in_read_set);
	fd_set* write_ptr = FillSetFromVector(write, in_write_set);
	fd_set* except_ptr = FillSetFromVector(except, in_except_set);

	int count = select(0, read_ptr, write_ptr, except_ptr, nullptr);

	if (count > 0)
	{
		FillVectorFromSet(out_read_set, in_read_set, read);
		FillVectorFromSet(out_write_set, in_write_set, write);
		FillVectorFromSet(out_except_set, in_except_set, except);
	}

	return count;
}
