#include <array>
#include <cstdint>
#include <filesystem>
#include <vector>

static constexpr std::size_t PERIOD_SIZE = 16;
using byte_t = std::uint8_t;
using mask_t = std::array<byte_t, PERIOD_SIZE>;
using table_t = std::array<mask_t, 256>;

struct CommandLineParameters
{
    std::filesystem::path input_filename;
    std::filesystem::path output_filename;
};

CommandLineParameters process_command_line_parameters(int argc, char* argv[]);

std::vector<byte_t> read_from_file(std::filesystem::path filename);

void store_to_file(std::filesystem::path filename, const std::vector<byte_t>& contents);

void process_data(std::vector<byte_t>& data, const table_t& table);
