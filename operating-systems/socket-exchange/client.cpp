/* Ковалев Даниил СКБ171. Вариант 9. Клиент.
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

std::vector<std::string> get_subdirs()
{
    const std::string COMMAND = "find . -maxdepth 1 -mindepth 1 -type d | cut -b3-";
    std::istringstream output(capture_cmd_out(COMMAND));
    std::vector<std::string> subdirs;
    std::string tmp;
    while (std::getline(output, tmp))
        subdirs.emplace_back(tmp);

    return subdirs;
}

int main() try
{
    std::cout << "CLIENT" << std::endl;

    sockaddr_in client_addr = get_client_addr();
    Socket listen_sock{};
    int sockoptval = 1;
    ::setsockopt(listen_sock.sfd, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof (sockoptval));
    bind_wrapper(listen_sock.sfd, client_addr);
    listen_wrapper(listen_sock.sfd);

    sockaddr_in server_addr = get_server_addr();
    Socket server_sock{};
    connect_wrapper(server_sock.sfd, server_addr);

    std::vector<std::string> subdirs = get_subdirs();
    send_size(server_sock.sfd, subdirs.size());
    for (const std::string& subdir : subdirs)
        send_string(server_sock.sfd, subdir);

    AcceptSocket client_sock(listen_sock.sfd);

    std::cout << "Subdirs count: " << subdirs.size() << '\n';
    for (std::size_t i = 0; i < subdirs.size(); ++i)
        std::cout << subdirs[i] << " has " << recv_size(client_sock.sfd) << " files\n";

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
