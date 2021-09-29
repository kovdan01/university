#include "shared.hpp"
#include <fstream>
#include <iterator>

CommandLineParameters process_command_line_parameters(int argc, char* argv[])
{
    if (argc != 3)
    {
        throw std::invalid_argument("Error: you must pass 2 integers as CLI-arguments");
    }

    return { std::filesystem::path(argv[1]), std::filesystem::path(argv[2]) };
}

std::vector<byte_t> read_from_file(std::filesystem::path filename)
{
    if (!std::filesystem::is_regular_file(filename))
    {
        throw std::invalid_argument("Error: input file does not seem to be a regular file");
    }
    std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
    file.unsetf(std::ios::skipws);
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    std::streampos input_file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<byte_t> contents;
    contents.reserve(input_file_size);
    contents.insert(contents.begin(), std::istream_iterator<byte_t>(file), std::istream_iterator<byte_t>());
    return contents;
}

void store_to_file(std::filesystem::path filename, const std::vector<byte_t>& contents)
{
    std::ofstream file(filename, std::ios_base::out | std::ios_base::binary);
    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
}

void process_data(std::vector<byte_t>& data, const table_t& table)
{
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        data[i] = table[data[i]][i % PERIOD_SIZE];
    }
}
