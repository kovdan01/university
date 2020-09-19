#include "shared.h"

#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <array>

constexpr int PROJECT_ID = 8888;
const std::string PATH_MEM = "/tmp/lab_shmem_mem";
const std::string PATH_SEM = "/tmp/lab_shmem_sem";

key_t get_key_impl(const std::string& path)
{
    ::creat(path.c_str(), 0600);
    key_t key = ::ftok(path.c_str(), PROJECT_ID);
    if (key == -1)
    {
        std::perror("Cannot get a key with ftok");
        throw std::runtime_error("Cannot get a key with ftok");
    }
    return key;
}

key_t get_key_mem()
{
    return get_key_impl(PATH_MEM);
}

key_t get_key_sem()
{
    return get_key_impl(PATH_SEM);
}

std::string capture_cmd_out(const std::string& cmd)
{
    std::string data;
    FILE* stream;
    const int max_buffer = 1024;
    char buffer[max_buffer];
    std::string new_cmd = cmd + " 2>&1";

    stream = ::popen(new_cmd.c_str(), "r");
    if (stream != nullptr)
    {
        while (std::feof(stream) == 0)
            if (std::fgets(buffer, max_buffer, stream) != nullptr)
                data.append(buffer);
        ::pclose(stream);
    }
    return data;
}

int create_shmem()
{
    key_t key = get_key_mem();
    int shm_id = ::shmget(key, SHMEM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id == -1)
    {
        std::perror("shmget error");
        throw std::runtime_error("shmget error");
    }

    return shm_id;
}

int open_shmem()
{
    key_t key = get_key_mem();
    int shm_id = ::shmget(key, SHMEM_SIZE, 0666);
    while (shm_id == -1)
    {
        ::usleep(10'000);
        shm_id = ::shmget(key, SHMEM_SIZE, 0666);
    }

    return shm_id;
}

int create_sem()
{
    key_t key = get_key_sem();
    int sem_id = ::semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id == -1)
    {
        std::perror("semget error");
        throw std::runtime_error("semget error");
    }

    return sem_id;
}

int open_sem()
{
    key_t key = get_key_sem();
    int sem_id = ::semget(key, 1, 0666);
    while (sem_id == -1)
    {
        ::usleep(10'000);
        sem_id = ::semget(key, 1, 0666);
    }

    return sem_id;
}

void sem_add(int sem_id, short num)
{
    std::array<sembuf, 1> sops;
    sops.fill({.sem_num = 0, .sem_op = num, .sem_flg = 0 });
    ::semop(sem_id, sops.data(), sops.size());
}

byte_t* attach_memory(int shm_id)
{
    const byte_t* ERROR_PTR = reinterpret_cast<const byte_t*>(-1);
    byte_t* ptr = reinterpret_cast<byte_t*>(::shmat(shm_id, nullptr, 0));
    if (ptr == ERROR_PTR)
    {
        std::perror("shmat error");
        throw std::runtime_error("shmat error");
    }
    return ptr;
}
