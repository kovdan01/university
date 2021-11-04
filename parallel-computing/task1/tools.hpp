#ifndef PARALLEL_COMPUTING_TASK1_HPP_
#define PARALLEL_COMPUTING_TASK1_HPP_

#include <chrono>
#include <cstdint>
#include <cstdlib>

namespace my
{

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
    double ticks;
    double nanoseconds;
};

class Timer
{
public:
    Timer(BenchmarkResult& result, std::size_t iterations_count = 1)
        : m_result(result)
        , m_iterations_count(iterations_count)
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
            m_result.ticks = (ticks_after - m_ticks_before) / static_cast<double>(m_iterations_count);
            m_result.nanoseconds = duration_cast<nanoseconds>(time_after - m_time_before).count() / static_cast<double>(m_iterations_count);
        }
        catch (...)
        {
            std::exit(EXIT_FAILURE);
        }
    }

private:
    BenchmarkResult& m_result;
    std::size_t m_iterations_count;
    std::uint64_t m_ticks_before;
    std::chrono::high_resolution_clock::time_point m_time_before;
};

}  // namespace my

#endif  // PARALLEL_COMPUTING_TASK1_HPP_
