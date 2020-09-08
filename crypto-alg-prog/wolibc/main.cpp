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

void print_results(ssize_t value, ssize_t correct)
{
    const char* got = "Got ";
    const char* expected = ", expected ";

    mywrite(STDOUT, got, mystrlen(got));
    write_num<ssize_t>(STDOUT, value);
    mywrite(STDOUT, expected, mystrlen(expected));
    write_num<ssize_t>(STDOUT, correct);
    mywrite(STDOUT, "\n", 1);
}

int main()
{
    const char* hello = "Hello ";
    const char* wolibc = "without libc!\n";

    ssize_t r1 = mywrite(STDOUT, hello, mystrlen(hello));
    ssize_t r2 = mywrite(STDOUT, wolibc, mystrlen(wolibc));
    ssize_t r3 = mywrite(5, "X\n", 2); // 5 is a bad fd, an error will occur

    const char* results = "Results:\n";
    mywrite(STDOUT, results, mystrlen(results));

    print_results(r1, mystrlen(hello));
    print_results(r2, mystrlen(wolibc));
    print_results(r3, -9); // -9 stands for bad fd

    return 0;
}
