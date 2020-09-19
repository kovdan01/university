/* Ковалев Даниил СКБ171. Вариант 9. Сервер.
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

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

#include <sys/msg.h>

std::vector<pri_t> rcv_priorities(int mq_id)
{
    msgbuf_t msg1;
    if (::msgrcv(mq_id, &msg1, sizeof(std::size_t), 1, 0) == -1)
    {
        std::perror("msgrcv error");
        throw std::runtime_error("msgrcv error");
    }
    std::size_t priorities_count = read_data<std::size_t>(msg1.mtext);

    msgbuf_t msg2;
    if (::msgrcv(mq_id, &msg2, priorities_count * sizeof(pri_t), 2, 0) == -1)
    {
        std::perror("msgrcv error");
        throw std::runtime_error("msgrcv error");
    }
    std::vector<pri_t> priorities;
    for (std::size_t i = 0; i < priorities_count; ++i)
        priorities.emplace_back(read_data<pri_t>(msg2.mtext + sizeof(pri_t) * i));

    std::cout << "Processes count: " << priorities_count << ", priorities: " << std::endl;
    for (pri_t pri : priorities)
        std::cout << pri << ' ';
    std::cout << std::endl;

    assert(priorities_count != 0);

    return priorities;
}

void print_max_pri_info(pri_t max_pri)
{
    const std::string PS_COMMAND = "ps -e --no-headers -o tty,pri,user,pid | awk '{if ($1 != \"?\") {print $2,$3,$4}}'";
    std::istringstream ps_output(capture_cmd_out(PS_COMMAND));
    struct proc
    {
        pri_t pri;
        std::string user;
        pid_t pid;
    };

    std::vector<proc> procs;
    proc tmp;
    while (ps_output >> tmp.pri >> tmp.user >> tmp.pid)
    {
        if (tmp.pri == max_pri)
        {
            procs.emplace_back(tmp);
            break;
        }
    }

    if (procs.empty())
    {
        std::cerr << "No procs with PRI = " << max_pri << " found" << std::endl;
        return;
    }

    std::cout << "Process with max PRI = " << max_pri << ": PID = " << procs.front().pid << ", USER = " << procs.front().user << std::endl;
}

void print_last_msg_time(int mq_id)
{
    msqid_ds mq_struct;
    if (::msgctl(mq_id, IPC_STAT, &mq_struct) == -1)
    {
        std::perror("msgctl get info error");
        throw std::runtime_error("msgctl get info error");
    }
    std::time_t unix_time = mq_struct.msg_stime;
    std::cout << "Last msgsnd time: " << std::put_time(std::localtime(&unix_time), "%c %Z") << std::endl;
}

void delete_mq(int mq_id)
{
    if (::msgctl(mq_id, IPC_RMID, nullptr) == -1)
    {
        std::perror("msgctl delete mq error");
        throw std::runtime_error("msgctl delete mq error");
    }
}

int main() try
{
    std::cout << "SERVER" << std::endl;

    int mq_id = create_or_open_mq();

    std::vector<pri_t> priorities = rcv_priorities(mq_id);

    std::sort(priorities.begin(), priorities.end());
    pri_t max_pri = priorities.front();
    print_max_pri_info(max_pri);

    print_last_msg_time(mq_id);

    delete_mq(mq_id);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
