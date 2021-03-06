#include "types.h"

#define STDOUT 1

extern "C"
{

extern ssize_t mywrite(int fd, const void* buf, size_t count);

}

size_t mystrlen(const char* str)
{
    size_t cnt = 0;
    for (; *str != '\0'; ++cnt, ++str) {}
    return cnt;
}

template <typename Integer>
ssize_t write_num(int fd, Integer num)
{
    // assuming 4 decimal digits as max for each byte, 1 for '-'
    size_t BUF_SIZE = sizeof(Integer) * 4 + 1;
    // integer representation from less to most significant digits 
    unsigned char digits[BUF_SIZE];
    // string representation from most to less significant digits (to be printed)
    char chars[BUF_SIZE + 1];

    char* buf = chars;
    size_t digits_count = 0;
    int negative = 0;

    if (num == 0)
    {
        chars[0] = '0';
        chars[1] = '\0';
        digits_count = 1;
        goto print;
    }

    if (num < 0)
    {
        negative = 1;
        num *= -1;
    }

    while (num != 0)
    {
        digits[digits_count] = num % 10;
        num /= 10;
        ++digits_count;
    }

    if (negative)
    {
        *buf = '-';
        ++buf;
    }

    for (size_t i = digits_count; i != 0; --i)
    {
        *buf = digits[i - 1] + '0';
        ++buf;
    }
    *buf = '\0';

    print:
    return mywrite(fd, chars, digits_count + negative);
}

template <typename Integer>
ssize_t print_num(Integer num)
{
    return write_num(STDOUT, num);
}

ssize_t print(const char* str)
{
    return mywrite(STDOUT, str, mystrlen(str));
}

void print_results(ssize_t value, ssize_t correct)
{
    print("Got ");
    print_num<ssize_t>(value);
    print(", expected ");
    print_num<ssize_t>(correct);
    print("\n");
}

int main()
{
    const char* hello = "Hello ";
    const char* wolibc = "without libc!\n";

    ssize_t r1 = print(hello);
    ssize_t r2 = print(wolibc);
    ssize_t r3 = mywrite(5, "X\n", 2); // 5 is a bad fd, an error will occur

    print("Results:\n");

    print_results(r1, mystrlen(hello));
    print_results(r2, mystrlen(wolibc));
    print_results(r3, -9); // -9 stands for bad fd

    return 0;
}
