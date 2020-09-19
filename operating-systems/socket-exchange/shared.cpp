#include "shared.h"

#include <sys/ipc.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <array>
#include <cstring>

std::string capture_cmd_out(const std::string& cmd)
{
    std::string data;
    FILE* stream;
    const int max_buffer = 1024;
    char buffer[max_buffer];
    std::string new_cmd = cmd + " 2>&1";

    stream = ::popen(new_cmd.c_str(), "r");
    if (stream != nullptr)
    {
        while (std::feof(stream) == 0)
            if (std::fgets(buffer, max_buffer, stream) != nullptr)
                data.append(buffer);
        ::pclose(stream);
    }
    return data;
}

int create_socket()
{
    int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        std::perror("socket create error");
        throw std::runtime_error("socket create error");
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

sockaddr_in get_addr_impl(in_port_t port)
{
    sockaddr_in addr;

    std::memset(&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);
    addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");

    return addr;
}

sockaddr_in get_server_addr()
{
    return get_addr_impl(6666);
}

sockaddr_in get_client_addr()
{
    return get_addr_impl(7777);
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

void send_size(int sfd, std::size_t num)
{
    byte_t buf[sizeof (std::size_t)];
    write_data<std::size_t>(num, buf);
    if (::send(sfd, buf, sizeof (std::size_t), 0) == -1)
    {
        std::perror("send error");
        throw std::runtime_error("send error");
    }
}

void send_string(int sfd, const std::string& str)
{
    send_size(sfd, str.size());
    if (::send(sfd, str.data(), str.size(), 0) == -1)
    {
        std::perror("send error");
        throw std::runtime_error("send error");
    }
}

std::size_t recv_size(int sfd)
{
    byte_t buf[sizeof (std::size_t)];
    if (::recv(sfd, buf, sizeof (std::size_t), 0) == -1)
    {
        std::perror("recv error");
        throw std::runtime_error("recv error");
    }
    return read_data<std::size_t>(buf);
}

std::string recv_string(int sfd, std::size_t len)
{
    std::string buf(len, '\0');
    if (::recv(sfd, buf.data(), len, 0) == -1)
    {
        std::perror("recv error");
        throw std::runtime_error("recv error");
    }
    return buf;
}
