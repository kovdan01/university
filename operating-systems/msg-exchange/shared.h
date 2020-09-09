#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <sys/types.h>

#include <cstddef>
#include <string>

key_t get_key();

constexpr std::size_t MSGSZ = 1024;

using pri_t = int;

struct msgbuf_t
{
    long mtype;
    unsigned char mtext[MSGSZ];
};

std::string capture_cmd_out(const std::string& cmd);

template <typename T>
T read_data(const unsigned char* from)
{
    T ans = 0;
    for (std::size_t shift = 0; shift != sizeof(T) * 8; shift += 8, ++from)
        ans |= static_cast<T>(*from) << shift;
    return ans;
}

template <typename T>
void write_data(T what, unsigned char* where)
{
    T mask = 0xff;
    for (std::size_t shift = 0; shift != sizeof(T) * 8; mask <<= 8, ++where, shift += 8)
        *where = (what & mask) >> shift;
}

#endif // CONSTANTS_H
