#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>

using byte_t = unsigned char;

std::string capture_cmd_out(const std::string& cmd);

template <typename T>
T read_data(const byte_t* from)
{
    T ans = 0;
    for (std::size_t shift = 0; shift != sizeof(T) * 8; shift += 8, ++from)
        ans |= static_cast<T>(*from) << shift;
    return ans;
}

template <typename T>
void write_data(T what, byte_t* where)
{
    T mask = 0xff;
    for (std::size_t shift = 0; shift != sizeof(T) * 8; mask <<= 8, ++where, shift += 8)
        *where = (what & mask) >> shift;
}

int create_socket();
void close_socket(int sfd);

struct Socket
{
    Socket() : sfd(create_socket()) {}

    ~Socket()
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

    const int sfd;
};

struct AcceptSocket
{
    AcceptSocket(int fd)
        : sfd(::accept(fd, nullptr, nullptr))
    {
        if (sfd == -1)
        {
            std::perror("accept error");
            throw std::runtime_error("accept error");
        }
    }

    ~AcceptSocket()
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

    const int sfd;
};

sockaddr_in get_server_addr();
sockaddr_in get_client_addr();

void bind_wrapper(int sfd, const sockaddr_in& addr);
void listen_wrapper(int sfd);
void connect_wrapper(int sfd, const sockaddr_in& addr);

void send_size(int sfd, std::size_t num);
void send_string(int sfd, const std::string& str);
std::size_t recv_size(int sfd);
std::string recv_string(int sfd, std::size_t len);

#endif // CONSTANTS_H
