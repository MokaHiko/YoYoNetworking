#pragma once

#include "SocketAddress.h"

class TCPSocket
{
public:
    ~TCPSocket();

    int Connect(const SocketAddress& in_address);

    // Binds socket to the address
    int Bind(const SocketAddress& in_address);

    // Used for servers to receive initial stage of handshake with max requests as back logs
    int Listen(int in_back_log = 32);

    // To accept an incoming connection and continue handshake. Creates a socket for that conection.
    std::shared_ptr<TCPSocket> Accept(SocketAddress& in_from_address);

    // Sends data to remote host and returns bytes sent
    int Send(const void* in_data, int length);

    // Receives data from remote host and returns bytes received
    int Receive(void* in_data, int length);

    int ToggleNonBlocking(bool is_non_blocking);
private:
    friend class SocketUtil;
    TCPSocket(SOCKET in_socket);
    SOCKET m_socket;
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;