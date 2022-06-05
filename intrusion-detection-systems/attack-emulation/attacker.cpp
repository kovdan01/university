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
    if (argc != 4)
        throw std::runtime_error("Error! Format: ./attacker task9 <defender_ip> <malware or legal>");

    std::string_view task = argv[1];
    std::string_view defender_ip = argv[2];
    std::string_view content_type = argv[3];

    if (task == "task7")
    {
        if (content_type != "malware" && content_type != "legal1" && content_type != "legal2")
            throw std::runtime_error("Error! Last argument must be either malware or legal1 or legal2.");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 7777);
        connect_wrapper(defender_sock.sfd, defender_addr);

        if (content_type == "malware")
        {
            std::array<char, 80> malware_buf = { '.', '/', 'i', 'n', 's', 't', 'a', 'l',
                                                 'l', '.', 'e', 'x', 'e', 'a', 'b', 'c',
                                                 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                                                 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                                                 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A',
                                                 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                                                 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                                 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
                                                 'u', 's', 'e', 'r', '0', '1', '2', '3',
                                                 '4', '5', '6', '7', '8', '9', '.', '.'};
            ::send(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0);
        }
        else if (content_type == "legal1")
        {
            std::array<char, 80> legal_buf = { '.', '/', 'i', 'n', 's', 't', 'a', 'l',
                                               'l', '.', 'e', 'x', 'e', 'a', 'b', 'c',
                                               'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                                               'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                                               't', 'u', 'v', 'w', 'x', 'y', 'z', 'A',
                                               'B', 'C', 'u', 's', 'e', 'r', 'H', 'I',
                                               'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                               'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
                                               'Z', 's', 'e', 'r', '0', '1', '2', '3',
                                               '4', '5', '6', '7', '8', '9', '.', '.'};
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
        else
        {
            assert(content_type == "legal2");
            std::array<char, 80> legal_buf = { '.', '.', 'i', 'n', 's', 't', 'a', 'l',
                                               'l', '.', 'e', 'x', 'e', 'a', 'b', 'c',
                                               'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                                               'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                                               't', 'u', 'v', 'w', 'x', 'y', 'z', 'A',
                                               'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                                               'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                               'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
                                               'u', 's', 'e', 'r', '0', '1', '2', '3',
                                               '4', '5', '6', '7', '8', '9', '.', '.'};
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
    }
    else if (task == "task8")
    {
        if (content_type != "malware" && content_type != "legal1" && content_type != "legal2" && content_type != "legal3")
            throw std::runtime_error("Error! Last argument must be either malware or legal1 or legal2 or legal3.");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 8888);
        connect_wrapper(defender_sock.sfd, defender_addr);

        if (content_type == "malware")
        {
            std::array<char, 64> malware_buf = { '.', '/', 'i', 'n', 's', 't', 'a', 'l',
                                                 'l', '.', 'e', 'x', 'e', '3', 'A', '0',
                                                 '1', 'F', '2', '3', '1', 'C', 'A', 'F',
                                                 '8', 'B', 'C', '9', '3', 'F', 'F', '9',
                                                 '1', '6', 'B', 'B', '3', 'C', 'A', '9',
                                                 'B', 'D', '0', 'B', 'E', 'a', 'a', 'a',
                                                 'a', 'u', 's', 'e', 'r', 'a', 'a', 'a',
                                                 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
            ::send(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0);
        }
        else if (content_type == "legal1")
        {
            std::array<char, 64> legal_buf = { '.', '/', 'i', 'n', 's', 't', 'a', 'l',
                                               'l', '.', 'e', 'x', 'e', '3', 'A', '0',
                                               '1', 'F', '2', '3', '1', 'C', 'A', 'F',
                                               '8', 'B', 'C', '9', '3', 'F', 'F', '9',
                                               '1', '6', 'B', 'B', '3', 'C', 'A', '9',
                                               'B', 'D', '0', 'B', 'E', 'a', 'a', 'a',
                                               'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
                                               'a', 'u', 's', 'e', 'r', 'a', 'a', 'a' };
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
        else if (content_type == "legal2")
        {
            std::array<char, 64> legal_buf = { '.', '.', 'i', 'n', 's', 't', 'a', 'l',
                                               'l', '.', 'e', 'x', 'e', '3', 'A', '0',
                                               '1', 'F', '2', '3', '1', 'C', 'A', 'F',
                                               '8', 'B', 'C', '9', '3', 'F', 'F', '9',
                                               '1', '6', 'B', 'B', '3', 'C', 'A', '9',
                                               'B', 'D', '0', 'B', 'E', 'a', 'a', 'a',
                                               'a', 'u', 's', 'e', 'r', 'a', 'a', 'a',
                                               'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
        else
        {
            assert(content_type == "legal3");
            std::array<char, 64> legal_buf = { '.', '/', 'i', 'n', 's', 't', 'a', 'l',
                                               'l', '.', 'e', 'x', 'e', '3', 'A', '0',
                                               '1', 'F', '2', '3', '1', 'X', 'A', 'F',
                                               '8', 'B', 'C', '9', '3', 'F', 'F', '9',
                                               '1', '6', 'B', 'B', '3', 'C', 'A', '9',
                                               'B', 'D', '0', 'B', 'E', 'a', 'a', 'a',
                                               'a', 'u', 's', 'e', 'r', 'a', 'a', 'a',
                                               'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' };
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
    }
    else if (task == "task9")
    {
        if (content_type != "malware" && content_type != "legal")
            throw std::runtime_error("Error! Last argument must be either malware or legal.");

        UdpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 4120);

        if (content_type == "malware")
        {
            std::array<uint8_t, 8> malware_buf = { 0x20, 0x00, 0xf1, 0x86, 0xab, 0x6d, 0x03, 0x30 };
            ::sendto(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0,
                     reinterpret_cast<struct sockaddr*>(&defender_addr), sizeof(defender_addr));
        }
        else
        {
            assert(content_type == "legal");
            std::array<uint8_t, 8> legal_buf = { 0x20, 0xfe, 0xf1, 0x86, 0xab, 0x6d, 0x03, 0x30 };
            ::sendto(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0,
                     reinterpret_cast<struct sockaddr*>(&defender_addr), sizeof(defender_addr));
        }
    }
    else if (task == "task11")
    {
        if (content_type != "malware" && content_type != "legal")
            throw std::runtime_error("Error! Last argument must be either malware or legal.");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 6000);
        connect_wrapper(defender_sock.sfd, defender_addr);

        if (content_type == "malware")
        {
            std::array<uint8_t, 16> malware_buf = { 0xaa, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbb, 0xcc, 0xdd, 0xee };
            ::send(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0);
        }
        else
        {
            assert(content_type == "legal");
            std::array<uint8_t, 16> legal_buf = { 0xaa, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0xbb, 0xcc, 0xdd, 0xee };
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
    }
    else if (task == "task12")
    {
        if (content_type != "malware" && content_type != "legal")
            throw std::runtime_error("Error! Last argument must be either malware or legal.");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 12754);
        connect_wrapper(defender_sock.sfd, defender_addr);

        if (content_type == "malware")
        {
            std::array<char, 4> malware_buf = { 'a', '>', 'x', 'c' };
            ::send(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0);
        }
        else
        {
            assert(content_type == "legal");
            std::array<char, 4> legal_buf = { 'a', '<', 'x', 'c' };
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
    }
    else if (task == "task13")
    {
        if (content_type != "malware" && content_type != "legal")
            throw std::runtime_error("Error! Last argument must be either malware or legal.");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 7597);
        connect_wrapper(defender_sock.sfd, defender_addr);

        if (content_type == "malware")
        {
            std::array<char, 16> malware_buf = { 'a', 'b', 'c', 'd', 'q', 'a', 'z', 'w', 's', 'x', '.', 'h', 's', 'q', 'x', 'y' };
            ::send(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0);
        }
        else
        {
            assert(content_type == "legal");
            std::array<char, 16> legal_buf = { 'a', 'b', 'c', 'd', 'q', 'a', 'z', 'w', 's', 'x', 'x', 'h', 's', 'q', 'x', 'y' };
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
    }
    else if (task == "task14")
    {
        if (content_type != "malware" && content_type != "legal1" && content_type != "legal2")
            throw std::runtime_error("Error! Last argument must be either malware or legal1 or legal2.");

        TcpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 1414);
        connect_wrapper(defender_sock.sfd, defender_addr);

        if (content_type == "malware")
        {
            std::array<char, 40> malware_buf = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                                                 'i', 'j', '=', 'P', '4', 'C', 'K', '3',
                                                 'T', '=', 'k', 'l', 'm', 'n', 'o', 'p',
                                                 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                                 'y', 'z', '8', '_', '=', '_', '8', '.'};
            ::send(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0);
        }
        else if (content_type == "legal1")
        {
            std::array<char, 40> legal_buf = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                                               'i', 'j', '=', 'P', '4', 'C', 'K', '4',
                                               'T', '=', 'k', 'l', 'm', 'n', 'o', 'p',
                                               'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                               'y', 'z', '8', '_', '=', '_', '8', '.'};
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
        else
        {
            assert(content_type == "legal2");
            std::array<char, 40> legal_buf = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                                               'i', 'j', '=', 'P', '4', 'C', 'K', '3',
                                               'T', '=', 'k', 'l', 'm', 'n', 'o', 'p',
                                               'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                               'y', 'z', '8', '_', '^', '_', '8', '.'};
            ::send(defender_sock.sfd, legal_buf.data(), legal_buf.size(), 0);
        }
    }
    else if (task == "task15")
    {
        if (content_type != "malware" && content_type != "legal1" && content_type != "legal2")
            throw std::runtime_error("Error! Last argument must be either malware or legal1 or legal2.");

        UdpSocket defender_sock;
        sockaddr_in defender_addr = get_addr_impl(defender_ip, 1900);

        if (content_type == "malware")
        {
            std::array<char, 24> malware_buf = { 'N', 'O', 'T', 'I', 'F',    'Y', ' ', '*',
                                                 ' ', 'x', 'x', 'x', 'L',    'O', 'C', 'A',
                                                 'T', 'I', 'O', 'N', '\x3a', 'a', 'a', 'a' };
            for (int i = 0; i < 11; ++i)
            {
                ::sendto(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0,
                         reinterpret_cast<struct sockaddr*>(&defender_addr), sizeof(defender_addr));
            }
        }
        else if (content_type == "legal1")
        {
            std::array<char, 24> malware_buf = { 'N', 'O', 'T', 'I', 'F',    'Y', ' ', '*',
                                                 ' ', 'x', 'x', 'x', 'L',    'O', 'C', 'A',
                                                 'T', 'I', 'O', 'N', '\x3a', 'a', 'a', 'a' };
            for (int i = 0; i < 9; ++i)
            {
                ::sendto(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0,
                         reinterpret_cast<struct sockaddr*>(&defender_addr), sizeof(defender_addr));
            }
        }
        else
        {
            assert(content_type == "legal2");
            std::array<char, 24> malware_buf = { 'N', 'O', 'T', 'I', 'F',    'Y', ' ', '*',
                                                 ' ', 'x', 'x', 'x', 'L',    'O', 'C', 'A',
                                                 'T', 'I', 'O', 'N', '\x3b', 'a', 'a', 'a' };
            for (int i = 0; i < 11; ++i)
            {
                ::sendto(defender_sock.sfd, malware_buf.data(), malware_buf.size(), 0,
                         reinterpret_cast<struct sockaddr*>(&defender_addr), sizeof(defender_addr));
            }
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
