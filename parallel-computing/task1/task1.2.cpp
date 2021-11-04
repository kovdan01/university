#include "tools.hpp"

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using arithmetic_function_t = std::function<double(double)>;

struct IntegrateParams
{
    arithmetic_function_t f;
    double dx;
    double from;
    double to;
};

using integrate_function_t = std::function<double(IntegrateParams)>;

double integrate_dummy(IntegrateParams params)
{
    asm volatile("# integrate_dummy enter");
    double sum = 0;
    for (double x = params.from; x < params.to; x += params.dx)
    {
        sum += params.dx * params.f(x);
    }
    asm volatile("# integrate_dummy exit");
    return sum;
}

double integrate_omp(IntegrateParams params)
{
    asm volatile("# integrate_omp enter");
    double sum = 0;
    std::size_t n = (params.to - params.from) / params.dx;
    #pragma omp parallel for reduction(+ : sum)
    for (std::size_t i = 0; i != n; ++i)
    {
        sum += params.dx * params.f(params.from + i * params.dx);
    }
    asm volatile("# integrate_omp exit");
    return sum;
}

my::BenchmarkResult measure_integrate(integrate_function_t integrate, IntegrateParams params)
{
    static constexpr std::size_t ITERATIONS_COUNT = 10'000;
    std::vector<my::BenchmarkResult> results(ITERATIONS_COUNT);
    for (my::BenchmarkResult& result : results)
    {
        my::Timer timer(result);
        double integrate_result = integrate(params);
        my::do_not_optimize(integrate_result);
    }
    my::BenchmarkResult results_sum = std::accumulate(results.begin(), results.end(), my::BenchmarkResult{ 0, 0 },
                                                      [](const my::BenchmarkResult& a, const my::BenchmarkResult& b) -> my::BenchmarkResult
                                                      {
                                                          return { .ticks = a.ticks + b.ticks,
                                                                   .nanoseconds = a.nanoseconds + b.nanoseconds };
                                                      });
    return { .ticks = results_sum.ticks / ITERATIONS_COUNT,
             .nanoseconds = results_sum.nanoseconds / ITERATIONS_COUNT };
}

void print_table_row(std::string_view label, my::BenchmarkResult result)
{
    std::cout << "| " << label << " | "
              << std::setw(14) << std::setprecision(std::numeric_limits<double>::digits10 + 1) << result.ticks << " | "
              << std::setw(13)  << std::setprecision(std::numeric_limits<double>::digits10 + 1) << result.nanoseconds << " |" << std::endl
              << "+--------------------+----------------+---------------+" << std::endl;
}

int main() try
{
    auto arithmetic_function = [](double x)
    {
        return std::exp(std::sin(std::pow(x, M_PI)));
    };
    IntegrateParams params =
    {
        .f = arithmetic_function,
        .dx = 0.0001,
        .from = -43.54325,
        .to = 34.6354
    };

    my::BenchmarkResult integrate_dummy_result = measure_integrate(integrate_dummy, params);
    my::BenchmarkResult integrate_omp_result = measure_integrate(integrate_omp, params);

    std::cout << "+--------------------+----------------+---------------+" << std::endl
              << "|      operation     |  ticks / iter  |   ns / iter   |" << std::endl
              << "+--------------------+----------------+---------------+" << std::endl;
    print_table_row(" integrate dummy  ", integrate_dummy_result);
    print_table_row("  integrate omp   ", integrate_omp_result);

    return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\nAborting\n";
    return EXIT_FAILURE;
}
