#include "tools.hpp"

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using arithmetic_function_t = std::function<double(double)>;
using function_values_table_t = std::vector<double>;

using integrate_function_t = std::function<double(const function_values_table_t& table, double dx)>;

double integrate_dummy(const function_values_table_t& table, double dx)
{
    asm volatile("# integrate_dummy enter");
    double sum = 0;
    for (double value : table)
    {
        sum += dx * value;
    }
    asm volatile("# integrate_dummy exit");
    return sum;
}

double integrate_omp_parallel(const function_values_table_t& table, double dx)
{
    asm volatile("# integrate_omp_parallel enter");
    double sum = 0;
    #pragma omp parallel for reduction(+ : sum)
    for (std::size_t i = 0; i != table.size(); ++i)
    {
        sum += dx * table[i];
    }
    asm volatile("# integrate_omp_parallel exit");
    return sum;
}

double integrate_omp_simd(const function_values_table_t& table, double dx)
{
    asm volatile("# integrate_omp_simd enter");
    double sum = 0;
    #pragma omp simd reduction(+:sum)
    for (std::size_t i = 0; i != table.size(); ++i)
    {
        sum += dx * table[i];
    }
    asm volatile("# integrate_omp_simd exit");
    return sum;
}

double arithmetic_function(double x)
{
    return std::exp(std::sin(std::pow(x, M_PI)));
};

my::BenchmarkResult measure_integrate(integrate_function_t integrate, const function_values_table_t& table, double dx)
{
    static constexpr std::size_t ITERATIONS_COUNT = 10'000'000;
    std::vector<my::BenchmarkResult> results(ITERATIONS_COUNT);
    for (my::BenchmarkResult& result : results)
    {
        my::Timer timer(result);
        double integrate_result = integrate(table, dx);
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
              << std::setw(14) << std::setprecision(2) << std::fixed << result.ticks << " | "
              << std::setw(13)  << std::setprecision(2) << std::fixed << result.nanoseconds << " |" << std::endl
              << "+--------------------------+----------------+---------------+" << std::endl;
}

std::vector<double> generate_function_values_table(arithmetic_function_t f, double from, double to, double dx)
{
    std::vector<double> table;
    table.reserve((to - from) / dx);
    for (double x = from; x <= to; x += dx)
    {
        table.emplace_back(f(x));
    }
    return table;
}

int main() try
{
    double dx = 0.0001;
    function_values_table_t table = generate_function_values_table(arithmetic_function, -43.54325, 34.6354, dx);

    my::BenchmarkResult integrate_dummy_result = measure_integrate(integrate_dummy, table, dx);
    my::BenchmarkResult integrate_omp_parallel_result = measure_integrate(integrate_omp_parallel, table, dx);
    my::BenchmarkResult integrate_omp_simd_result = measure_integrate(integrate_omp_simd, table, dx);

    std::cout << "+--------------------------+----------------+---------------+" << std::endl
              << "|         operation        |  ticks / iter  |   ns / iter   |" << std::endl
              << "+--------------------------+----------------+---------------+" << std::endl;
    print_table_row("    integrate dummy     ", integrate_dummy_result);
    print_table_row(" integrate omp parallel ", integrate_omp_parallel_result);
    print_table_row("   integrate omp simd   ", integrate_omp_simd_result);

    return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\nAborting\n";
    return EXIT_FAILURE;
}
