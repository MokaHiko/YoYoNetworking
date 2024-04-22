#pragma once

#include "UDPSocket.h"
#include "TCPSocket.h"

#define NO_ERROR 0

enum class SocketAddressFamily
{
    INET = AF_INET,
    INET6 = AF_INET6,
};

class SocketUtil
{
public:
    static int GetLastError();

    static int StartUp();

    // Creates a udp socket
    static UDPSocketPtr CreateUDPSocket(SocketAddressFamily in_family);

    // Creates a tcp socket
    static TCPSocketPtr CreateTCPSocket(SocketAddressFamily in_family);

    // Fills a vector of sockets that are contained in a fd set from a set of sockets
    static fd_set* FillSetFromVector(fd_set& out_set, const std::vector<TCPSocketPtr>* in_sockets);

    // Fills a vector of sockets that are contained in a fd set from a set of sockets
    static fd_set* FillSetFromVector(fd_set& out_set, const std::vector<UDPSocketPtr>* in_sockets);

    // Fills a vector of sockets that are contained in a fd set from a set of sockets
    static void FillVectorFromSet(std::vector<TCPSocketPtr>* out_sockets, const std::vector<TCPSocketPtr>* in_sockets, const fd_set& in_set);

    // Fills a vector of sockets that are contained in a fd set from a set of sockets
    static void FillVectorFromSet(std::vector<UDPSocketPtr>* out_sockets, const std::vector<UDPSocketPtr>* in_sockets, const fd_set& in_set);

    static int Select(const std::vector<TCPSocketPtr>* in_read_set, std::vector<TCPSocketPtr>* out_read_set,
        const std::vector<TCPSocketPtr>* in_write_set, std::vector<TCPSocketPtr>* out_write_set,
        const std::vector<TCPSocketPtr>* in_except_set, std::vector<TCPSocketPtr>* out_except_set);

    static int Select(long time_interval, const std::vector<TCPSocketPtr>* in_read_set, std::vector<TCPSocketPtr>* out_read_set,
        const std::vector<TCPSocketPtr>* in_write_set, std::vector<TCPSocketPtr>* out_write_set,
        const std::vector<TCPSocketPtr>* in_except_set, std::vector<TCPSocketPtr>* out_except_set);

    static int Select(const std::vector<UDPSocketPtr>* in_read_set, std::vector<UDPSocketPtr>* out_read_set,
        const std::vector<UDPSocketPtr>* in_write_set, std::vector<UDPSocketPtr>* out_write_set,
        const std::vector<UDPSocketPtr>* in_except_set, std::vector<UDPSocketPtr>* out_except_set);

    static void Shutdown();
};
