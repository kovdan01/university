#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using BYTE = std::int8_t;
using WORD = std::int16_t;

void lab_byte(BYTE* A, BYTE* B, std::uint64_t* C, std::uint8_t& swapped_count)
{
    asm(".intel_syntax          \n\t"
        "mov rdi, rax           \n\t" // адрес A в RDI
        "mov rsi, rbx           \n\t" // адрес B в RSI
        "mov r8,  rcx           \n\t" // адрес C в R8
        "xor dl,  dl            \n\t" // обнуляем счетчик обменов
        "mov ecx, 16            \n\t"
        "start_b:               \n\t" // начало цикла
        "mov al,  [rdi]         \n\t" // байт из массива A
        "cmp al,  [rsi]         \n\t" // сравниваем с байтом из массива B
        "jle after_swap_b       \n\t" // если A[i] <= B[i], ничего делать не надо
        "mov bl,  [rsi]         \n\t" // меняем местами A[i] и B[i]
        "mov [rdi],  bl         \n\t"
        "mov [rsi],  al         \n\t"
        "inc dl                 \n\t" // увеличиваем счетчик обменов
        //"mov ax,  16            \n\t" // (16 - CX) в AX: текущий индекс
        //"sub ax,  cx            \n\t"
        "mov [r8], rdi          \n\t" // текущий адрес A в массив C
        "add r8, 8              \n\t"
        "mov [r8], rsi          \n\t" // текущий адрес A в массив C
        "add r8, 8              \n\t"
        "after_swap_b:          \n\t"
        "inc rdi                \n\t"
        "inc rsi                \n\t"
        "loop start_b"
        : "=d"(swapped_count)
        : "a"(A), "b"(B), "c"(C)
        );
}

void lab_word(WORD* A, WORD* B, std::uint64_t* C, std::uint8_t& swapped_count)
{
    asm(".intel_syntax          \n\t"
        "mov rdi, rax           \n\t" // адрес A в RDI
        "mov rsi, rbx           \n\t" // адрес B в RSI
        "mov r8,  rcx           \n\t" // адрес C в R8
        "xor dl,  dl            \n\t" // обнуляем счетчик обменов
        "mov ecx, 16            \n\t"
        "start_w:               \n\t" // начало цикла
        "mov ax,  [rdi]         \n\t" // слово из массива A
        "cmp ax,  [rsi]         \n\t" // сравниваем с словом из массива B
        "jle after_swap_w       \n\t" // если A[i] <= B[i], ничего делать не надо
        "mov bx,  [rsi]         \n\t" // меняем местами A[i] и B[i]
        "mov [rdi],  bx         \n\t"
        "mov [rsi],  ax         \n\t"
        "inc dl                 \n\t" // увеличиваем счетчик обменов
        //"mov ax,  16            \n\t" // (16 - CX) в AX: текущий индекс
        //"sub ax,  cx            \n\t"
    "mov [r8], rdi          \n\t" // текущий адрес A в массив C
    "add r8, 8              \n\t"
    "mov [r8], rsi          \n\t" // текущий адрес A в массив C
    "add r8, 8              \n\t"
        //"mov [r8], al           \n\t" // текущий индекс в массив C
        //"inc r8                 \n\t"
        "after_swap_w:          \n\t"
        "add rdi, 2             \n\t"
        "add rsi, 2             \n\t"
        "loop start_w"
        : "=d"(swapped_count)
        : "a"(A), "b"(B), "c"(C)
        );
}

template<typename T>
void print_all(const std::vector<T>& A, const std::vector<T>& B)
{
    std::cout << "A: {";
    for (T elem : A)
        std::cout << std::dec << std::setw(6) << static_cast<int>(elem) << ' ';
    std::cout << "}\nB: {";
    for (T elem : B)
        std::cout << std::dec << std::setw(6) << static_cast<int>(elem) << ' ';
    std::cout << "}\n";
}

template<typename T>
void print_all(const std::vector<T>& A, const std::vector<T>& B, const std::vector<std::uint64_t> C, std::uint8_t swapped_count)
{
    print_all(A, B);
    std::cout << "\nC: {\n";
    for (std::uint8_t i = 0; i < swapped_count * 2; i += 2)
        std::cout << "     " << std::dec << std::setw(2) << (C[i] - reinterpret_cast<std::uint64_t>(A.data())) / sizeof(T) << ' '
                  << std::hex << C[i] << ' ' << C[i + 1] << '\n';
    std::cout << "   }\nSwapped number: " << std::dec << static_cast<int>(swapped_count) << '\n';
}

int main()
{
    std::cout << "i: {";
    for (int i = 0; i < 16; ++i)
        std::cout << std::setw(6) << i << ' ';
    std::vector<std::uint64_t> C(32);
    std::uint8_t swapped_count;
    std::cout << "}\n";
    {
        std::cout << "\n" + std::string(55, ' ') + "BYTE\n";
        std::vector<BYTE> A = {  105,   70,   90,   82, -114,   45,   59,   82,  -13,   35, -123,  -70,   57,  -98,   81,  -32 };
        std::vector<BYTE> B = {  -20,  -55,   32,  -10,   39,  -57,   27,   69,  -29,  -81,  123,   34, -118,  -96,   44, -110 };

        std::cout << "Before:\n";
        print_all(A, B);
        lab_byte(A.data(), B.data(), C.data(), swapped_count);
        std::cout << "\nAfter:\n";
        print_all(A, B, C, swapped_count);
    }
    {
        std::cout << "\n" + std::string(55, ' ') + "WORD\n";
        std::vector<WORD> A = {   4996, -28469,  12215, -14279, -22535, -16077, -16993, -26883,    510,   4745,  -4753,   3704, -30365,  30061,  31930,  22819 };
        std::vector<WORD> B = {  25757, -20781,  -4543,  31013, -32147,  -2348,  22367,  27115,  30568,   6958,  25501,  23278, -25810,  22947,   4479,   1446 };

        std::cout << "Before:\n";
        print_all(A, B);
        lab_word(A.data(), B.data(), C.data(), swapped_count);
        std::cout << "\nAfter:\n";
        print_all(A, B, C, swapped_count);
    }
}
