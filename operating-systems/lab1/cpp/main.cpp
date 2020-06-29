#include <iostream>
#include <string>
#include <csignal>
#include <cassert>
#include <csetjmp>
#include <unistd.h>

static sigjmp_buf proc_info;

[[noreturn]] void sigint_handler(int signal_code)
{
    assert(signal_code == SIGINT);
    static int interrupt_count = 0;
    ++interrupt_count;
    std::cout << "\n";
    if (interrupt_count > 5)
    {
        std::cout << "List of *.c files in this catalogue:\n";
        std::system("ls -a *.c");
    }
    siglongjmp(proc_info, 1);
}

int main()
{
    struct sigaction handler;
    handler.sa_handler = sigint_handler;
    sigprocmask(0, nullptr, &handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGINT, &handler, nullptr);

    while (true)
    {
        sigsetjmp(proc_info, 1);
        std::string c_filename;
        std::cout << "Enter file name: ";
        std::getline(std::cin, c_filename);

        sigsetjmp(proc_info, 1);
        close(STDERR_FILENO);
        std::cout << "[INFO] Compiling \"" << c_filename << "\"...\n";
        std::string cmd = "gcc \"" + c_filename + "\" -o \"" + c_filename + ".out\"";
        int exit_status = WEXITSTATUS(std::system(cmd.c_str()));
        if (exit_status != 0)
        {
            std::cout << "[INFO] Error while compiling " << c_filename << "\n";
            continue;
        }

        sigsetjmp(proc_info, 1);
        std::cout << "[INFO] Running...\n";
        cmd = "./\"" + c_filename + ".out\"";
        std::system(cmd.c_str());

        sigsetjmp(proc_info, 1);
        std::cout << "[INFO] Deleting executable...\n";
        unlink(std::string(c_filename + ".out").c_str());
        //cmd = "rm \"" + c_filename + ".out\"";
        //std::system(cmd.c_str());
    }
}
