#pragma once

#include "SocketAddress.h"

class UDPSocket
{
public:
    ~UDPSocket();

    // Binds socket to the address
    int Bind(const SocketAddress& in_bind_address);

    // Sends data of size length in bytes to address;
    int SendTo(const void* in_data, int length, const SocketAddress& in_to_address);

    // Receives data from and fills address. 
    int ReceiveFrom(void* in_buffer, int length, SocketAddress& out_from_address);
    
    int ToggleNonBlocking(bool is_non_blocking);
private:
    friend class SocketUtil;
    UDPSocket(SOCKET socket);
    SOCKET m_socket;
};

typedef std::shared_ptr<UDPSocket> UDPSocketPtr;