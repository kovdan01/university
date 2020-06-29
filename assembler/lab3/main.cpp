#include <iostream>

void lab(char x, char y, char z)
{
    std::cout << "Input:  " <<
                 "x = " << int{x} << ", "
                 "y = " << int{y} << ", "
                 "z = " << int{z} << '\n';
    asm(".intel_syntax   \n\t"
        "mov  dh, al    ;\n\t" // x в DH
        "mov  bh, 5     ;\n\t" // 5 в BH
        "xor  ah, ah    ;\n\t"
        "div  bh        ;\n\t" // в AL частное от деления x на 5, в AH - остаток
        "cmp  bl, al    ;\n\t" // сравниваем частное и y
        "jne  change    ;\n\t"
        "cmp  cl, ah    ;\n\t" // сравниваем остаток и z
        "jne  change    ;\n\t"
        "cmp  cl, 0     ;\n\t" // если z ноль, то уменьшать не нужно
        "je   exit      ;\n\t"
        "sub  cl, 1     ;\n\t" // уменьшаем z на 1
        "jmp  exit      ;\n\t"
        "change:         \n\t"
        "mov  bl, dh    ;\n\t" // x в BL
        "mov  cl, dh    ;\n\t" // x в CL
        "exit:           \n\t"
        "mov  al, dh    ;\n\t" // x в AL
        "xor  ah, ah    ;\n\t" // убираем мусор
        "xor  bh, bh    ;\n\t"
        : "=a"(x), "=b"(y), "=c"(z)
        : "a"(x), "b"(y), "c"(z)
    );
    std::cout << "Result: " <<
                 "x = " << int{x} << ", "
                 "y = " << int{y} << ", "
                 "z = " << int{z} << '\n';
}

int main()
{
    char x, y, z;
    for (;;)
    {
        std::cout << "Enter a string of 3 decimal digits: ";
        std::cin >> x >> y >> z;
        if (!std::isdigit(x) || !std::isdigit(y) || !std::isdigit(z))
        {
            std::cerr << "At least one of " << x << ", " << y << ", " << z << " is not a digit!\n";
            continue;
        }
        lab(x - '0', y - '0', z - '0');
    }
}
