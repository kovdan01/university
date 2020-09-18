/* Ковалев Даниил СКБ171. Вариант 9. Сервер.
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
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip>

void delete_shmem(int shm_id)
{
    if (::shmctl(shm_id, IPC_RMID, nullptr) == -1)
    {
        std::perror("shmctl delete error");
        throw std::runtime_error("shmctl delete error");
    }
}

void delete_sem(int sem_id)
{
    if (::semctl(sem_id, 0, IPC_RMID) == -1)
    {
        std::perror("semctl delete error");
        throw std::runtime_error("semctl delete error");
    }
}

struct Resources
{
    Resources()
        : shm_id(create_shmem())
        , sem_id(create_sem())
    {
    }

    ~Resources()
    {
        try
        {
            delete_shmem(shm_id);
            delete_sem(sem_id);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception caught in dtor: " << e.what() << std::endl;
        }
    }

    const int shm_id;
    const int sem_id;
};

std::vector<pid_t> rcv_pids(byte_t* ptr)
{
    std::size_t pids_count = read_data<std::size_t>(ptr);
    ptr += sizeof(std::size_t);

    std::vector<pid_t> pids;
    for (std::size_t i = 0; i < pids_count; ++i, ptr += sizeof(pid_t))
        pids.emplace_back(read_data<pid_t>(ptr));

    std::cout << "Processes count: " << pids.size() << ", PIDs: " << std::endl;
    for (pid_t pid : pids)
        std::cout << pid << ' ';
    std::cout << std::endl;

    return pids;
}

std::vector<pid_t> get_pris(const std::vector<pid_t>& pids)
{
    if (pids.empty())
        return {};

    std::string ps_cmd = "ps --no-headers -o pri -p ";
    for (pid_t pid : pids)
        ps_cmd += std::to_string(pid) + ",";
    ps_cmd.pop_back();
    std::istringstream ps_output(capture_cmd_out(ps_cmd));
    std::vector<pid_t> pris;
    pid_t tmp;
    while (ps_output >> tmp)
        pris.emplace_back(tmp);

    std::cout << "Processes count: " << pids.size() << ", PRIs: " << std::endl;
    for (pri_t pri : pris)
        std::cout << pri << ' ';
    std::cout << std::endl;

    return pris;
}

void snd_data(byte_t* ptr, const std::vector<pri_t>& pris, std::time_t semop_time)
{
    write_data<std::size_t>(pris.size(), ptr);
    ptr += sizeof(std::size_t);

    for (std::size_t i = 0; i < pris.size(); ++i, ptr += sizeof(pri_t))
        write_data<pri_t>(pris[i], ptr);

    write_data<std::time_t>(semop_time, ptr);
}

std::time_t get_last_semop_time(int sem_id)
{
    union semun
    {
       int val;
       semid_ds* buf;
       unsigned short* array;
    };
    semid_ds sem_struct;
    if (::semctl(sem_id, 0, IPC_STAT, semun{.buf = &sem_struct}) == -1)
    {
        std::perror("semctl get info error");
        throw std::runtime_error("semctl get info error");
    }
    std::time_t unix_time = sem_struct.sem_otime;
    std::cout << "Last semop time: " << std::put_time(std::localtime(&unix_time), "%c %Z") << std::endl;

    return unix_time;
}

int main() try
{
    std::cout << "SERVER" << std::endl;
    Resources res{}; // init semaphore and shared memory
    byte_t* ptr = attach_memory(res.shm_id);

    sem_add(res.sem_id, -1); // wait for client message

    std::vector<pid_t> pids = rcv_pids(ptr);
    std::vector<pri_t> pris = get_pris(pids);
    std::time_t semop_time = get_last_semop_time(res.sem_id);

    snd_data(ptr, pris, semop_time);

    sem_add(res.sem_id, 2);

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
