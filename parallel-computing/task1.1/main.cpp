#include <immintrin.h>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

static_assert (sizeof(long long) == sizeof(std::int64_t), "long long and std::int64_t do not match");
static_assert (sizeof(__m256i) % sizeof(std::int64_t) == 0, "Size of __m256i is not multiple of size of std::int64_t");
static constexpr std::size_t SCALARS_IN_VECTOR = sizeof(__m256i) / sizeof(std::int64_t);

static constexpr std::size_t ITERATIONS_COUNT = 100'000'000;

template <typename T>
inline __attribute__((always_inline)) void do_not_optimize(T& value)
{
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

inline __attribute__((always_inline)) std::uint64_t ticks()
{
    std::uint64_t tsc;
    asm volatile("mfence; "         // memory barrier
                 "rdtsc; "          // read of tsc
                 "shl $32,%%rdx; "  // shift higher 32 bits stored in rdx up
                 "or %%rdx,%%rax"   // and or onto rax
                 : "=a"(tsc)        // output to tsc
                 :
                 : "%rcx", "%rdx", "memory");
    return tsc;
}

struct BenchmarkResult
{
    double ticks_per_iteration;
    double nanoseconds_per_iteration;
};

class Timer
{
public:
    Timer(BenchmarkResult& result)
        : m_result(result)
    {
        m_time_before = std::chrono::high_resolution_clock::now();
        m_ticks_before = ticks();
    }

    ~Timer()
    {
        try
        {
            using namespace std::chrono;
            std::uint64_t ticks_after = ticks();
            high_resolution_clock::time_point time_after = high_resolution_clock::now();
            m_result.ticks_per_iteration = (ticks_after - m_ticks_before) / static_cast<double>(ITERATIONS_COUNT);
            m_result.nanoseconds_per_iteration = duration_cast<nanoseconds>(time_after - m_time_before).count() / static_cast<double>(ITERATIONS_COUNT);
        }
        catch (...)
        {
            std::exit(EXIT_FAILURE);
        }
    }

private:
    BenchmarkResult& m_result;
    std::uint64_t m_ticks_before;
    std::chrono::high_resolution_clock::time_point m_time_before;
};

BenchmarkResult independent_scalar_operation(std::int64_t n1, std::int64_t n2)
{
    asm volatile("# ticks_per_independent_scalar_operation enter");
    BenchmarkResult result;
    {
        Timer timer(result);
        for (std::size_t i = 0; i < ITERATIONS_COUNT; ++i)
        {
            std::int64_t n3 = n1 / n2;
            do_not_optimize(n3);
        }
    }
    return result;
    asm volatile("# ticks_per_independent_scalar_operation exit");
}

BenchmarkResult dependent_scalar_operation(std::int64_t n1, std::int64_t n2)
{
    asm volatile("# ticks_per_dependent_scalar_operation enter");
    BenchmarkResult result;
    {
        Timer timer(result);
        for (std::size_t i = 0; i < ITERATIONS_COUNT; ++i)
        {
            n1 /= n2;
        }
        do_not_optimize(n1);
    }
    return result;
    asm volatile("# ticks_per_dependent_scalar_operation exit");
}

void put_scalar_into_vector(std::int64_t scalar, __m256i& vector)
{
    std::int64_t array[SCALARS_IN_VECTOR];
    for (std::size_t i = 0; i < SCALARS_IN_VECTOR; ++i)
        array[i] = scalar;
    std::memcpy(&vector, array, sizeof(vector));
}

BenchmarkResult independent_vector_operation(std::int64_t n1, std::int64_t n2)
{
    asm volatile("# ticks_per_independent_vector_operation enter\n");

    __m256i v1, v2;
    put_scalar_into_vector(n1, v1);
    put_scalar_into_vector(n2, v2);

    static constexpr std::size_t vector_iterations_count = ITERATIONS_COUNT / SCALARS_IN_VECTOR;
    BenchmarkResult result;
    {
        Timer timer(result);
        for (std::size_t i = 0; i < vector_iterations_count; ++i)
        {
            __m256i v3 = _mm256_div_epi64(v1, v2);
            do_not_optimize(v3);
        }
    }
    return result;
    asm volatile("# ticks_per_independent_vector_operation exit");
}

BenchmarkResult dependent_vector_operation(std::int64_t n1, std::int64_t n2)
{
    asm volatile("# ticks_per_dependent_vector_operation enter\n");

    __m256i v1, v2;
    put_scalar_into_vector(n1, v1);
    put_scalar_into_vector(n2, v2);

    static constexpr std::size_t vector_iterations_count = ITERATIONS_COUNT / SCALARS_IN_VECTOR;
    BenchmarkResult result;
    {
        Timer timer(result);
        for (std::size_t i = 0; i < vector_iterations_count; ++i)
        {
            v1 = _mm256_div_epi64(v1, v2);
        }
        do_not_optimize(v1);
    }
    return result;
    asm volatile("# ticks_per_dependent_vector_operation exit");
}

struct Params
{
    std::int64_t n1;
    std::int64_t n2;
};

Params parse_cmd_line(int argc, char* argv[])
{
    if (argc != 3)
    {
        throw std::invalid_argument("Error: you must pass 2 integers as CLI-arguments");
    }

    auto int_from_string = [](const std::string& s) -> std::int64_t
    {
        try
        {
            std::size_t pos;
            int n = std::stoll(s, &pos);
            if (pos != s.size())
            {
                throw std::invalid_argument("stoll");
            }
            return n;
        }
        catch (const std::invalid_argument& e)
        {
            throw std::invalid_argument("Error: cannot convert \"" + s + "\" to std::int64_t: \"" + e.what() + "\"");
        }
        catch (const std::out_of_range& e)
        {
            throw std::out_of_range("Error: value \"" + s + "\" is out of range of type std::int64_t: \"" + e.what() + "\"");
        }
    };

    return Params{ .n1 = int_from_string(std::string(argv[1])),
                   .n2 = int_from_string(std::string(argv[2])) };
}

void print_table_row(std::string_view label, BenchmarkResult result)
{
    std::cout << "| " << label << " | "
              << std::setw(12) << result.ticks_per_iteration << " | "
              << std::setw(9)  << result.nanoseconds_per_iteration << " |" << std::endl
              << "+--------------------+--------------+-----------+" << std::endl;
}

int main(int argc, char* argv[]) try
{
    Params params = parse_cmd_line(argc, argv);

    BenchmarkResult independent_scalar_result = independent_scalar_operation(params.n1, params.n2);
    BenchmarkResult dependent_scalar_result = dependent_scalar_operation(params.n1, params.n2);
    BenchmarkResult independent_vector_result = independent_vector_operation(params.n1, params.n2);
    BenchmarkResult dependent_vector_result = dependent_vector_operation(params.n1, params.n2);

    std::cout << "+--------------------+--------------+-----------+" << std::endl
              << "|      operation     | ticks / iter | ns / iter |" << std::endl
              << "+--------------------+--------------+-----------+" << std::endl;
    print_table_row("independent scalar", independent_scalar_result);
    print_table_row(" dependent scalar ", dependent_scalar_result);
    print_table_row("independent vector", independent_vector_result);
    print_table_row(" dependent vector ", dependent_vector_result);

    return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\nAborting\n";
    return EXIT_FAILURE;
}
