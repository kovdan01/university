/* Ковалев Даниил СКБ171. Вариант 9. Клиент.
 *
 * Обмен через разделяемую память
 *
 * Сервер. Создать разделяемую область памяти (РОП) и набор семафоров (НС).
 * При поступлении клиентского сообщения в РОП определить системную
 * составляющую приоритета  полученных процессов, а также время когда
 * последний процесс выполнял операции над семафорами и переслать клиенту.
 * Удалить РОП и НС.
 *
 * Клиент. Подсоединиться к РОП. Определить идентификаторы процессов,
 * являющихся системными и переслать эти данные в серверное гнездо. При
 * получении ответа от сервера, распечатать поступившую информацию.
 */

#include "shared.h"

#include <sys/shm.h>

#include <iostream>
#include <stdexcept>
#include <array>
#include <functional>
#include <sstream>
#include <ctime>
#include <iomanip>

std::vector<pid_t> get_pids()
{
    const std::string PS_COMMAND = "ps -e --no-headers -o tty,pid | awk '{if ($1 == \"?\") {print $2}}'";
    std::istringstream ps_output(capture_cmd_out(PS_COMMAND));
    std::vector<pid_t> pids;
    pid_t tmp;
    while (ps_output >> tmp)
        pids.emplace_back(tmp);

    std::cout << "Processes count: " << pids.size() << ", PIDs: " << std::endl;
    for (pid_t pid : pids)
        std::cout << pid << ' ';
    std::cout << std::endl;

    return pids;
}

void snd_pids(const std::vector<pid_t>& pids, byte_t* ptr)
{
    write_data<std::size_t>(pids.size(), ptr);
    ptr += sizeof(std::size_t);

    for (std::size_t i = 0; i < pids.size(); ++i, ptr += sizeof(pid_t))
        write_data<pid_t>(pids[i], ptr);
}

void rcv_data(byte_t* ptr)
{
    std::size_t pris_count = read_data<std::size_t>(ptr);
    ptr += sizeof(std::size_t);

    std::vector<pid_t> pris;
    for (std::size_t i = 0; i < pris_count; ++i, ptr += sizeof(pri_t))
        pris.emplace_back(read_data<pri_t>(ptr));

    std::time_t semop_time = read_data<std::time_t>(ptr);

    std::cout << "Processes count: " << pris.size() << ", PRIs: " << std::endl;
    for (pid_t pri : pris)
        std::cout << pri << ' ';
    std::cout << std::endl;

    std::cout << "Last semop time: " << std::put_time(std::localtime(&semop_time), "%c %Z") << std::endl;
}

int main() try
{
    std::cout << "CLIENT" << std::endl;

    int shm_id = open_shmem();
    int sem_id = open_sem();

    byte_t* ptr = attach_memory(shm_id);
    std::vector<pid_t> pids = get_pids();
    snd_pids(pids, ptr);

    sem_add(sem_id, 1); // signal for server about new message

    sem_add(sem_id, -2); // wait for server response

    rcv_data(ptr);

    if (::shmdt(ptr) == -1)
    {
        std::perror("shmdt");
        throw std::runtime_error("shmdt");
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
