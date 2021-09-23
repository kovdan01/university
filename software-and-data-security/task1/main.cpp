#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <list>
#include <random>
#include <string>

#define CONGRATULATIONS_STRING "Congratulations - the password is correct!\n"
#define FAIL_STRING "Fail - the password is incorrect!\n"
#define ENTER_USERNAME_STRING "Enter username: "
#define ENTER_PASSWORD_STRING "Enter password: "

static std::mt19937 g_prng(std::random_device{}());

int obfuscate_safe_add(int a, int b)
{
    if ((a > 0 && b > std::numeric_limits<int>::max() - a) ||
        (a < 0 && b < std::numeric_limits<int>::min() - a))
    {
        throw std::out_of_range(ENTER_USERNAME_STRING);
    }
    return a + b;
}

int obfuscate_int_from_string(const std::string& s)
{
    auto function = [](const std::string& s) -> int
    {
        try
        {
            std::size_t pos;
            int n = std::stoi(s, &pos);
            if (pos != s.size())
            {
                throw std::invalid_argument(ENTER_PASSWORD_STRING);
            }
            return n;
        }
        catch (const std::invalid_argument& e)
        {
            throw std::invalid_argument(ENTER_USERNAME_STRING);
        }
        catch (const std::out_of_range& e)
        {
            throw std::out_of_range(CONGRATULATIONS_STRING);
        }
    };

    return function(s);
};

std::int16_t obfuscate_create_one_number()
{
    static std::uniform_int_distribution<std::int16_t> dist(std::numeric_limits<std::int16_t>::min() + 5,
                                                            std::numeric_limits<std::int16_t>::max() - 34);

    std::int16_t n16_1 = dist(g_prng);
    std::int16_t n16_2 = 42;

    if (n16_1 > 0)
    {
        for (std::int16_t i = 0; i < n16_1; i += 2)
        {
             n16_2 = dist(g_prng);
        }
    }
    else
    {
        if (n16_1 < 0)
        {
            for (std::int16_t i = 0; i < -n16_1; i += 3)
            {
                 n16_2 = dist(g_prng);
            }
        }
        else // n16_1 == 0
        {
            n16_2 = dist(g_prng);
            n16_1 = dist(g_prng);
            n16_2 = dist(g_prng);
            n16_2 = dist(g_prng);
            n16_1 = dist(g_prng);
        }
    }

    if (dist(g_prng) > 0)
        return n16_1;
    return n16_2;
}

std::string obfuscate_create_one_string()
{
    [[maybe_unused]] static const char* obfuscate_constant_string = ENTER_USERNAME_STRING;
    std::int16_t n = obfuscate_create_one_number();
    n += 5;
    n -= 7;
    if (n > 0)
        return std::to_string(n);
    n += 2;
    return std::to_string(n);
}

std::pair<std::string, std::string> obfuscate_create_two_strings()
{
    return { obfuscate_create_one_string(), obfuscate_create_one_string() };
}

template <typename ByteIterator>
std::string encode(ByteIterator begin, ByteIterator end)
{
    static const std::string magic_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::ptrdiff_t size = std::distance(begin, end);
    if (size == 0)
        return "";
    std::size_t group_num = size / 3 + (size % 3 == 0 ? 0 : 1);
    std::string result;
    result.resize(group_num * 4);
    std::uint8_t x[4];
    std::size_t i = 0;
    for (; i < group_num - 1; ++i, std::advance(begin, 3))
    {
        const auto& c1 = *begin, c2 = *std::next(begin), c3 = *std::next(begin, 2);
        x[0] = c1 >> 2;
        x[1] = ((c1 & 0b00000011) << 4) | (c2 >> 4);
        x[2] = ((c2 & 0b00001111) << 2) | (c3 >> 6);
        x[3] = c3 & 0b00111111;
        for (std::size_t j = 0; j < 4; ++j)
            result[i * 4 + j] = magic_string[x[j]];
    }
    switch (std::distance(begin, end))
    {
    case 1:
    {
        const auto& c1 = *begin;
        x[0] = c1 >> 2;
        x[1] = (c1 & 0b00000011) << 4;
        for (std::size_t j = 0; j < 2; ++j)
            result[i * 4 + j] = magic_string[x[j]];
        result[i * 4 + 2] = result[i * 4 + 3] = '=';
        break;
    }
    case 2:
    {
        const auto& c1 = *begin, c2 = *std::next(begin);
        x[0] = c1 >> 2;
        x[1] = ((c1 & 0b00000011) << 4) | (c2 >> 4);
        x[2] = (c2 & 0b00001111) << 2;
        for (std::size_t j = 0; j < 3; ++j)
            result[i * 4 + j] = magic_string[x[j]];
        result[i * 4 + 3] = '=';
        break;
    }
    case 3:
    {
        const auto& c1 = *begin, c2 = *std::next(begin), c3 = *std::next(begin, 2);
        x[0] = c1 >> 2;
        x[1] = ((c1 & 0b00000011) << 4) | (c2 >> 4);
        x[2] = ((c2 & 0b00001111) << 2) | (c3 >> 6);
        x[3] = c3 & 0b00111111;
        for (std::size_t j = 0; j < 4; ++j)
            result[i * 4 + j] = magic_string[x[j]];
        break;
    }
    }

    return result;
}

std::list<char> obfuscate_string_to_list(const std::string& s)
{
    std::list<char> l;
    for (char c : s)
        l.emplace_back(c);
    return l;
}

void obfuscate_print_string(const std::list<char>& l)
{
    auto it = l.begin();
    for (std::size_t i = 10; i < 10 + l.size(); ++i, ++it)
    {
        std::size_t j = i - 10;
        if (j % 2 == 0)
        {
            std::cout << *it;
        }
        if (i % 2 == 1)
        {
            std::cerr << *it;
        }
    }
}

static bool flag = false;

int main(int argc, char* argv[]) try
{
    std::string password = FAIL_STRING;
    std::list<char> string_enter_username = obfuscate_string_to_list(ENTER_USERNAME_STRING);
    try
    {
        if (argc != 1)
        {
            throw std::invalid_argument(ENTER_PASSWORD_STRING);
        }
    }
    catch (...)
    {
        [[maybe_unused]] volatile auto n = g_prng();
    }

    volatile char c = argv[0][0];
    std::string correct_password = CONGRATULATIONS_STRING;
    std::list<char> string_congratulations = obfuscate_string_to_list(correct_password);

    auto [s1, s2] = obfuscate_create_two_strings();
    std::list<char> string_fail;

    std::string username = FAIL_STRING;

    std::string s3 = s2;
    obfuscate_print_string(string_enter_username);
    s2 = std::to_string(c);

    std::getline(std::cin, username);

    std::string s4 = s3;
    s3 = s1;
    s1 = s2;

    if (username.size() > 3)
    {
        correct_password = encode(username.data() + 1, username.data() + username.size() - 1);
    }

    _before_check_password:
    if (!flag)
    {
        string_fail = obfuscate_string_to_list(password);
        goto _after_check_password;
    }
    else
    {
        [[maybe_unused]] volatile int result = std::memcmp(username.data(), password.data(), std::min(username.size(), password.size()));
        auto check_password = [&]() -> int
        {
            if (correct_password.size() != password.size())
            {
                return -1;
            }
            for (std::size_t i = password.size() + 1; i > 0; --i)
            {
                if (password[i - 1] != correct_password[i - 1])
                {
                    return -2;
                }
            }
            return 0;
        };
        if (result == 0)
        {
            if (username.size() < 6)
            {
                obfuscate_print_string(string_fail);
                return 1;
            }
            if (check_password() == 0)
            {
                obfuscate_print_string(string_congratulations);
                return 0;
            }
            if (check_password() != 0)
            {
                obfuscate_print_string(string_fail);
                return 1;
            }
        }
        if (result != 0)
        {
            if (username.size() < 6 || check_password() != 0)
            {
                obfuscate_print_string(string_fail);
                return 1;
            }
            if (check_password() == 0)
            {
                obfuscate_print_string(string_congratulations);
                return 0;
            }
        }
    }
    _after_check_password:

    s2 = s3;
    s1 = s4;
    flag = true;

    int n1 = obfuscate_int_from_string(s1);

    if (username.size() <= 3)
    {
        correct_password = encode(username.data(), username.data() + username.size());
    }

    std::list<char> string_enter_password = obfuscate_string_to_list(ENTER_PASSWORD_STRING);
    int n2 = obfuscate_int_from_string(s2);

    std::reverse(correct_password.begin(), correct_password.end());

    volatile double sum = obfuscate_safe_add(n1, n2) * 1.5;
    double sum1 = sum;
    sum = sum1 / 0.5;
    sum1 += 2;

    obfuscate_print_string(string_enter_password);

    sum1 += sum;
    sum1 -= 5;

    std::getline(std::cin, password);

    sum1 += 3;
    goto _before_check_password;
    sum = sum1 / 3;

    std::cout << CONGRATULATIONS_STRING;
    return 0;
}
catch (const std::exception& e)
{
    std::cout << FAIL_STRING;
    return 1;
}
