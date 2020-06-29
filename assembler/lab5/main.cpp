#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using BYTE = std::int8_t;
using WORD = std::int16_t;

void lab_byte(BYTE* A)
{
    asm(".intel_syntax          \n\t"
        "mov rdi, rax           \n\t" // адрес A в RDI
        "mov ecx, 16            \n\t"
        "start_b:               \n\t" // начало цикла
        "mov al,  [rdi]         \n\t" // байт из массива A в AL
        "mov dl,  al            \n\t" // байт из массива A в DL
        "shr al,  7             \n\t" // старший бит в AL
        "and dl,  1             \n\t" // младший бит в DL
        "cmp al,  dl            \n\t"
        "jne after_copy_b       \n\t" // если ст. и мл. биты не равны, ничего не делать
        "mov al,  [rdi]         \n\t" // байт из массива A в AL
        "mov dl,  al            \n\t" // байт из массива A в DL
        "and al,  231           \n\t" // обнуляем биты 4:3
        "and dl,  96            \n\t" // обнуляем биты, кроме 6:5
        "shr dl,  2             \n\t" // кладем биты 6:5 в биты 4:3
        "or  al,  dl            \n\t"
        "mov [rdi], al          \n\t" // изменяем элемент массива
        "after_copy_b:          \n\t"
        "inc rdi                \n\t" // переходим к следующему элементу
        "loop start_b           \n\t"
        :
        : "a"(A)
        );
}

void lab_word(WORD* A)
{
    asm(".intel_syntax          \n\t"
        "mov rdi, rax           \n\t" // адрес A в RDI
        "mov ecx, 16            \n\t"
        "start_w:               \n\t" // начало цикла
        "mov ax,  [rdi]         \n\t" // слово из массива A в AX
        "mov dx,  ax            \n\t" // слово из массива A в DX
        "shr ax,  15            \n\t" // старший бит в AX
        "and dx,  1             \n\t" // младший бит в DX
        "cmp ax,  dx            \n\t"
        "jne after_copy_w       \n\t" // если ст. и мл. биты не равны, ничего не делать
        "mov ax,  [rdi]         \n\t" // слово из массива A в AX
        "mov dx,  ax            \n\t" // слово из массива A в DX
        "and ax,  65511         \n\t" // обнуляем биты 4:3
        "and dx,  96            \n\t" // обнуляем биты, кроме 6:5
        "shr dx,  2             \n\t" // кладем биты 6:5 в биты 4:3
        "or  ax,  dx            \n\t"
        "mov [rdi], ax          \n\t" // изменяем элемент массива
        "after_copy_w:          \n\t"
        "add rdi, 2             \n\t" // переходим к следующему элементу
        "loop start_w           \n\t"
        :
        : "a"(A)
        );
}

void print_words(const std::vector<WORD>& A)
{
    std::cout << "{";
    for (WORD elem : A)
        std::cout << std::hex << std::setw(6) << (elem) << ' ';
    std::cout << "}\n";
}

void print_bytes(const std::vector<BYTE>& A)
{
    std::cout << "{";
    for (BYTE elem : A)
        std::cout << std::hex << std::setw(6) << (static_cast<int>(elem) & 255) << ' ';
    std::cout << "}\n";
}

int main()
{
    {
        std::cout << std::string(55, ' ') + "BYTE\n";
        std::vector<BYTE> A = {  105,   70,   90,   82, -114,   45,   59,   82,  -13,   35, -123,  -70,   57,  -98,   81,  -32 };

        std::cout << "Before:\n";
        print_bytes(A);
        lab_byte(A.data());
        std::cout << "After:\n";
        print_bytes(A);
    }
    {
        std::cout << "\n" + std::string(55, ' ') + "WORD\n";
        std::vector<WORD> A = {   4996, -28469,  12215, -14279, -22535, -16077, -16993, -26883,    510,   4745,  -4753,   3704, -30365,  30061,  31930,  22819 };
        std::vector<WORD> B = {  25757, -20781,  -4543,  31013, -32147,  -2348,  22367,  27115,  30568,   6958,  25501,  23278, -25810,  22947,   4479,   1446 };

        std::cout << "Before:\n";
        print_words(A);
        lab_word(A.data());
        std::cout << "After:\n";
        print_words(A);
    }
}
