#include "shared.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

int main(int argc, const char* argv[]) try
{
    if (argc < 2)
        throw std::runtime_error("Error! Format: ./defender taskX <task-specific-options>");

    std::string_view task = argv[1];
    if (task == "task9")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task9");

        UdpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 4120);
        bind_wrapper(defender_sock.sfd, defender_addr);

        std::cout << "Listening for UDP datagrams..." << std::endl;
        for (;;)
        {
            std::array<uint8_t, 8> buf;
            ::recv(defender_sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Datagram received." << std::endl;
        }
    }
    else if (task == "task11")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task11");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 6000);
        bind_wrapper(defender_sock.sfd, defender_addr);
        listen_wrapper(defender_sock.sfd);

        std::cout << "Listening for TCP packets..." << std::endl;
        for (;;)
        {
            AcceptSocket sock(defender_sock.sfd);
            std::array<uint8_t, 1024> buf;
            ::recv(sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Packet received." << std::endl;
        }
    }
    else if (task == "task12")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task12");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 12754);
        bind_wrapper(defender_sock.sfd, defender_addr);
        listen_wrapper(defender_sock.sfd);

        std::cout << "Listening for TCP packets..." << std::endl;
        for (;;)
        {
            AcceptSocket sock(defender_sock.sfd);
            std::array<uint8_t, 1024> buf;
            ::recv(sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Packet received." << std::endl;
        }
    }
    else if (task == "task13")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task13");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 7597);
        bind_wrapper(defender_sock.sfd, defender_addr);
        listen_wrapper(defender_sock.sfd);

        std::cout << "Listening for TCP packets..." << std::endl;
        for (;;)
        {
            AcceptSocket sock(defender_sock.sfd);
            std::array<uint8_t, 1024> buf;
            ::recv(sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Packet received." << std::endl;
        }
    }
    else if (task == "task14")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task14");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 1234);
        bind_wrapper(defender_sock.sfd, defender_addr);
        listen_wrapper(defender_sock.sfd);

        std::cout << "Listening for TCP packets..." << std::endl;
        for (;;)
        {
            AcceptSocket sock(defender_sock.sfd);
            std::array<uint8_t, 1024> buf;
            ::recv(sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Packet received." << std::endl;
        }
    }
    else if (task == "task15")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task15");

        UdpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 1900);
        bind_wrapper(defender_sock.sfd, defender_addr);

        std::cout << "Listening for UDP datagrams..." << std::endl;
        for (;;)
        {
            std::array<uint8_t, 24> buf;
            ::recv(defender_sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Datagram received." << std::endl;
        }
    }
    else if (task == "task16")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task16");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 1234);
        bind_wrapper(defender_sock.sfd, defender_addr);
        listen_wrapper(defender_sock.sfd);

        std::cout << "Listening for TCP packets..." << std::endl;
        for (;;)
        {
            AcceptSocket sock(defender_sock.sfd);
            std::array<uint8_t, 1024> buf;
            ::recv(sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Packet received." << std::endl;
        }
    }
    else if (task == "task17")
    {
        if (argc != 2)
            throw std::runtime_error("Error! Format: ./defender task17");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, 1234);
        bind_wrapper(defender_sock.sfd, defender_addr);
        listen_wrapper(defender_sock.sfd);

        std::cout << "Listening for TCP packets..." << std::endl;
        for (;;)
        {
            AcceptSocket sock(defender_sock.sfd);
            std::array<uint8_t, 1024> buf;
            ::recv(sock.sfd, buf.data(), buf.size(), 0);
            std::cout << "Packet received." << std::endl;
        }
    }
    else
    {
        throw std::runtime_error("Error! Task " + std::string(task) + " is not a valid task name.");
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
