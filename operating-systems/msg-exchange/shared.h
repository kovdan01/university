#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <sys/types.h>

#include <cstddef>
#include <string>

constexpr std::size_t MSGSZ = 1024;

using pri_t = int;
using byte_t = unsigned char;

struct msgbuf_t
{
    long mtype;
    byte_t mtext[MSGSZ];
};

int create_or_open_mq();

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

#endif // CONSTANTS_H
