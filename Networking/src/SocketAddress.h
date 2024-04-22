#pragma once

#ifdef _WIN32 
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

class SocketAddress
{
public:
    SocketAddress() = default;
    ~SocketAddress() = default;

    SocketAddress(uint32_t in_address, uint16_t in_port);
    SocketAddress(const sockaddr& in_sock_addr);

    // Returns the size of the socket address
    size_t GetSize() const {return sizeof(sockaddr);}
private:
    friend class UDPSocket;
    friend class TCPSocket;
    sockaddr m_socket_addr;
    sockaddr_in* GetAsSockAddrIn(){return reinterpret_cast<sockaddr_in*>(&m_socket_addr);}
};
typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory
{
public:
    static SocketAddressPtr CreateIPv4FromString(const std::string& in_string);
};