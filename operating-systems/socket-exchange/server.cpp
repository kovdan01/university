/* Ковалев Даниил СКБ171. Вариант 9. Сервер.
 *
 * Обмен через гнезда
 *
 * Сервер. Создать гнездо с установлением соединения домена INET. Присвоить
 * ему имя. При поступлении клиентского сообщения, определить количество
 * файлов в каждой поддиректории текущего каталога и переслать эту информацию
 * в клиентское гнездо.
 *
 * Клиент. Создать гнездо с установлением соединения INET. Переслать в
 * серверное гнездо сообщение обо всех поддиректориях текущего каталога.
 * Записать в стандартный файл вывода ответ сервера.
 */

#include "shared.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::size_t get_number_of_files_in_dir(const std::string& dir)
{
    const std::string COMMAND = "ls -1A " + dir + " | wc -l";
    std::istringstream output(capture_cmd_out(COMMAND));
    std::size_t cnt;
    output >> cnt;
    return cnt;
}

int main() try
{
    std::cout << "SERVER" << std::endl;

    sockaddr_in server_addr = get_server_addr();
    Socket listen_sock{};
    int sockoptval = 1;
    ::setsockopt(listen_sock.sfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof (sockoptval));
    bind_wrapper(listen_sock.sfd, server_addr);
    listen_wrapper(listen_sock.sfd);

    sockaddr_in client_addr = get_client_addr();
    Socket client_sock{};
    connect_wrapper(client_sock.sfd, client_addr);

    AcceptSocket server_sock(listen_sock.sfd);

    std::size_t subdirs_cnt = recv_size(server_sock.sfd);
    std::cout << "Subdirs count: " << subdirs_cnt << '\n';
    for (std::size_t i = 0; i < subdirs_cnt; ++i)
    {
        std::size_t name_len = recv_size(server_sock.sfd);
        std::string dir = recv_string(server_sock.sfd, name_len);
        std::size_t files_num = get_number_of_files_in_dir(dir);

        std::cout << dir << " has " << files_num << " files\n";
        send_size(client_sock.sfd, files_num);
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
