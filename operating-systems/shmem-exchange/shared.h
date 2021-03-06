#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <cstddef>
#include <string>

using pri_t = int;
using byte_t = unsigned char;

static constexpr std::size_t SHMEM_SIZE = 1 << 16;

std::string capture_cmd_out(const std::string& cmd);

template <typename T>
T read_data(const byte_t* from)
{
    T ans = 0;
    for (std::size_t shift = 0; shift != sizeof(T) * 8; shift += 8, ++from)
        ans |= static_cast<T>(*from) << shift;
    return ans;
}

template <typename T>
void write_data(T what, byte_t* where)
{
    T mask = 0xff;
    for (std::size_t shift = 0; shift != sizeof(T) * 8; mask <<= 8, ++where, shift += 8)
        *where = (what & mask) >> shift;
}

int create_shmem();
int open_shmem();

byte_t* attach_memory(int shm_id);

int create_sem();
int open_sem();

void sem_add(int sem_id, short num);

#endif // CONSTANTS_H
