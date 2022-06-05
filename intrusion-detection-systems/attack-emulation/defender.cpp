#include "shared.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

template <std::size_t buf_size = 1024>
static void listen_for_tcp_packets(in_port_t port)
{
    TcpSocket defender_sock;
    sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, port);
    bind_wrapper(defender_sock.sfd, defender_addr);
    listen_wrapper(defender_sock.sfd);

    std::cout << "Listening for TCP packets..." << std::endl;
    for (;;)
    {
        AcceptSocket sock(defender_sock.sfd);
        std::array<uint8_t, buf_size> buf;
        ::recv(sock.sfd, buf.data(), buf.size(), 0);
        std::cout << "Packet received." << std::endl;
    }
}

template <std::size_t buf_size>
static void listen_for_udp_datagrams(in_port_t port)
{
    UdpSocket defender_sock;
    sockaddr_in defender_addr = get_addr_impl(INADDR_ANY, port);
    bind_wrapper(defender_sock.sfd, defender_addr);

    std::cout << "Listening for UDP datagrams..." << std::endl;
    for (;;)
    {
        std::array<uint8_t, buf_size> buf;
        ::recv(defender_sock.sfd, buf.data(), buf.size(), 0);
        std::cout << "Datagram received." << std::endl;
    }
}

int main(int argc, const char* argv[]) try
{
    if (argc != 2)
        throw std::runtime_error("Error! Format: ./defender taskX");

    std::string_view task = argv[1];
    if (task == "task7")
        listen_for_tcp_packets(7777);
    else if (task == "task8")
        listen_for_tcp_packets(8888);
    else if (task == "task9")
        listen_for_udp_datagrams<8>(4120);
    else if (task == "task11")
        listen_for_tcp_packets(6000);
    else if (task == "task12")
        listen_for_tcp_packets(12754);
    else if (task == "task13")
        listen_for_tcp_packets(7597);
    else if (task == "task14")
        listen_for_tcp_packets(1414);
    else if (task == "task15")
        listen_for_udp_datagrams<24>(1900);
    else
        throw std::runtime_error("Error! Task " + std::string(task) + " is not a valid task name.");

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
