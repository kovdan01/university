#include <iostream>
#include <limits>
#include <string>

int safe_add(int a, int b)
{
    if ((a > 0 && b > std::numeric_limits<int>::max() - a) ||
        (a < 0 && b < std::numeric_limits<int>::min() - a))
    {
        throw std::out_of_range("Error: sum is out of range of type int");
    }
    return a + b;
}

int main(int argc, char* argv[]) try
{
    if (argc != 3)
    {
        throw std::invalid_argument("Error: you must pass 2 integers as CLI-arguments");
    }

    auto int_from_string = [](const std::string& s) -> int
    {
        try
        {
            std::size_t pos;
            int n = std::stoi(s, &pos);
            if (pos != s.size())
            {
                throw std::invalid_argument("stoi");
            }
            return n;
        }
        catch (const std::invalid_argument& e)
        {
            throw std::invalid_argument("Error: cannot convert \"" + s + "\" to int: \"" + e.what() + "\"");
        }
        catch (const std::out_of_range& e)
        {
            throw std::out_of_range("Error: value \"" + s + "\" is out of range of type int: \"" + e.what() + "\"");
        }
    };

    int n1 = int_from_string(std::string(argv[1]));
    int n2 = int_from_string(std::string(argv[2]));

    std::cout << n1 << " + " << n2 << " = " << safe_add(n1, n2) << '\n';
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\nAborting\n";
    return 1;
}
