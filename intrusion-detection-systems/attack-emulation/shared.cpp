#include "shared.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>

int create_tcp_socket()
{
    int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        std::perror("tcp socket create error");
        throw std::runtime_error("tcp socket create error");
    }
    return sfd;
}

int create_udp_socket()
{
    int sfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1)
    {
        std::perror("udp socket create error");
        throw std::runtime_error("udp socket create error");
    }
    return sfd;
}

void close_socket(int sfd)
{
    if (::close(sfd) == -1)
    {
        std::perror("socket close error");
        throw std::runtime_error("socket close error");
    }
}

sockaddr_in get_addr_impl(in_addr_t addr, in_port_t port)
{
    sockaddr_in address;

    std::memset(&address, 0, sizeof (address));
    address.sin_family = AF_INET;
    address.sin_port = ::htons(port);
    address.sin_addr.s_addr = addr;
    if (address.sin_addr.s_addr == INADDR_NONE)
    {
        std::perror("inet_addr error");
        throw std::runtime_error("inet_addr error");
    }

    return address;
}

sockaddr_in get_addr_impl(std::string_view addr, in_port_t port)
{
    in_addr_t address = ::inet_addr(addr.data());
    if (address == INADDR_NONE)
    {
        std::perror("inet_addr error");
        throw std::runtime_error("inet_addr error");
    }

    return get_addr_impl(address, port);
}

void bind_wrapper(int sfd, const sockaddr_in& addr)
{
    if (::bind(sfd, reinterpret_cast<const sockaddr*>(&addr), sizeof (addr)) == -1)
    {
        std::perror("bind error");
        throw std::runtime_error("bind error");
    }
}

void listen_wrapper(int sfd)
{
    if (::listen(sfd, 1) < 0)
    {
        std::perror("listen error");
        throw std::runtime_error("listen error");
    }
}

void connect_wrapper(int sfd, const sockaddr_in& addr)
{
    int res = ::connect(sfd, reinterpret_cast<const sockaddr*>(&addr), sizeof (addr));
    while (res == -1)
    {
        ::usleep(1'000);
        res = ::connect(sfd, reinterpret_cast<const sockaddr*>(&addr), sizeof (addr));
    }
}


UdpSocket::UdpSocket() : sfd(create_udp_socket()) {}

UdpSocket::~UdpSocket()
{
    try
    {
        close_socket(sfd);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

TcpSocket::TcpSocket() : sfd(create_tcp_socket()) {}

TcpSocket::~TcpSocket()
{
    try
    {
        close_socket(sfd);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

AcceptSocket::AcceptSocket(int fd)
    : sfd(::accept(fd, nullptr, nullptr))
{
    if (sfd == -1)
    {
        std::perror("accept error");
        throw std::runtime_error("accept error");
    }
}

AcceptSocket::~AcceptSocket()
{
    try
    {
        close_socket(sfd);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
