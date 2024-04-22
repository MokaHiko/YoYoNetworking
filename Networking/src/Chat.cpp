#include "Chat.h"
#include "SocketUtil.h"

int main()
{
    {
       
    }

    UDPSocketPtr server_socket = SocketUtil::CreateUDPSocket(SocketAddressFamily::INET);
    SocketAddressPtr server_address = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:6000");
    if (server_socket->Bind(*server_address))
    {
        printf("Server failed to bind to address");
    }

    char message_buffer[1024] = "NICE ASS";
    if (int n_bytes_sent = server_socket->SendTo((void*)message_buffer, 1024, *server_address) <= 0)
    {
        printf("Failed to send!");
    }

    char in_message_buffer[1024];
    memset(in_message_buffer, 0, sizeof(1024));
    int n_bytes = server_socket->ReceiveFrom((void*)in_message_buffer, 1024, *server_address);
    if(n_bytes <= 0)
    {
        printf("Failed to receieve packet!");
    }
    printf("%s", in_message_buffer);



    return 0;
}