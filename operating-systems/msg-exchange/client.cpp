/* Ковалев Даниил СКБ171. Вариант 9. Клиент.
 *
 * Обмен сообщениями
 *
 * Клиент. Создать очередь сообщений. Записать в качестве 1-ого сообщения
 * количество активных процессов, работающих в настоящее время в системе и
 * управляемых терминалами, в качестве 2-го сообщения – значения приоритетов
 * этих процессов.
 *
 * Сервер. Выбрать из очереди 2-ое сообщение и определить процесс с наивысшим
 * приоритетом, а также имя владельца этого процесса и время, когда это
 * сообщение было записано в очередь.
 */

#include "shared.h"

#include <sys/msg.h>
#include <sys/types.h>

#include <iostream>
#include <sstream>
#include <vector>

std::vector<pri_t> get_priorities()
{
    const std::string PS_COMMAND = "ps -e --no-headers -o tty,pri | awk '{if ($1 != \"?\") {print $2}}'";
    std::istringstream ps_output(capture_cmd_out(PS_COMMAND));
    std::vector<pri_t> priorities;
    pri_t tmp;
    while (ps_output >> tmp)
        priorities.emplace_back(tmp);

    std::cout << "Processes count: " << priorities.size() << ", priorities: " << std::endl;
    for (pri_t pri : priorities)
        std::cout << pri << ' ';
    std::cout << std::endl;

    return priorities;
}

void send_priorities(int mq_id, const std::vector<pri_t>& priorities)
{
    msgbuf_t msg1;
    msg1.mtype = 1;
    write_data<std::size_t>(priorities.size(), msg1.mtext);
    if (::msgsnd(mq_id, &msg1, sizeof(std::size_t), IPC_NOWAIT) == -1)
    {
        std::perror("msgsnd error: ");
        std::exit(3);
    }

    msgbuf_t msg2;
    msg2.mtype = 2;
    for (std::size_t i = 0; i < priorities.size(); ++i)
        write_data<pri_t>(priorities[i], msg2.mtext + sizeof(pri_t) * i);

    if (::msgsnd(mq_id, &msg2, sizeof(pri_t) * priorities.size(), IPC_NOWAIT) == -1)
    {
        std::perror("msgsnd error: ");
        std::exit(3);
    }
}

int main()
{
    std::cout << "CLIENT" << std::endl;

    int mq_id = create_or_open_mq();

    std::vector<pri_t> priorities = get_priorities();

    if (sizeof(pri_t) * priorities.size() > MSGSZ)
    {
        std::cerr << "Too many processes to fit their PRIs in one message" << std::endl;
        std::exit(0);
    }

    send_priorities(mq_id, priorities);
}
