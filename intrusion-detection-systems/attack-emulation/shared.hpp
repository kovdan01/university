#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string_view>

sockaddr_in get_addr_impl(in_addr_t addr, in_port_t port);
sockaddr_in get_addr_impl(std::string_view ip, in_port_t port);
void bind_wrapper(int sfd, const sockaddr_in& addr);
void listen_wrapper(int sfd);
void connect_wrapper(int sfd, const sockaddr_in& addr);

struct UdpSocket
{
    UdpSocket();
    ~UdpSocket();

    const int sfd;
};

struct TcpSocket
{
    TcpSocket();
    ~TcpSocket();

    const int sfd;
};

struct AcceptSocket
{
    AcceptSocket(int fd);
    ~AcceptSocket();

    const int sfd;
};
