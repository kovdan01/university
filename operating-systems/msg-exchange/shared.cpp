#include "shared.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <stdexcept>

constexpr int PROJECT_ID = 8888;
const std::string PATH = "/tmp/lab_mq";

key_t get_key()
{
    ::creat(PATH.c_str(), 0600);
    key_t key = ::ftok(PATH.c_str(), PROJECT_ID);
    if (key == -1)
    {
        std::perror("Cannot get a key with ftok");
        throw std::runtime_error("Cannot get a key with ftok");
    }
    return key;
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

int create_or_open_mq()
{
    key_t key = get_key();
    int mq_id = ::msgget(key, IPC_CREAT | 0666);
    if (mq_id == -1)
    {
        std::perror("MQ creation or open error");
        throw std::runtime_error("MQ creation or open error");
    }

    return mq_id;
}
