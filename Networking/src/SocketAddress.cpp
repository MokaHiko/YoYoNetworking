#include "SocketAddress.h"

SocketAddress::SocketAddress(uint32_t in_address, uint16_t in_port)
{
	// TODO: Support IPV6
	GetAsSockAddrIn()->sin_family = AF_INET;

	GetAsSockAddrIn()->sin_port = htons(in_port);
	GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(in_address);
}

SocketAddress::SocketAddress(const sockaddr& in_sock_addr)
{
	memcpy(&m_socket_addr, &in_sock_addr, sizeof(sockaddr));
}

SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const std::string& in_string)
{
	auto pos = in_string.find_last_of(':');
	std::string host, service;

	if (pos != std::string::npos)
	{
		host = in_string.substr(0, pos);
		service = in_string.substr(pos + 1);
	}
	else
	{
		host = in_string;

		// Using default service
		service = "0";
	}

	addrinfo hint;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	addrinfo* result;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

	if (error != 0 && result != nullptr)
	{
		freeaddrinfo(result);
		return nullptr;
	}

	// Find first valid result
	while (!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}

	if (!result->ai_addr)
	{
		freeaddrinfo(result);
		return nullptr;
	}

	SocketAddressPtr address = std::make_shared<SocketAddress>(*result->ai_addr);
	freeaddrinfo(result);
	return address;
};
