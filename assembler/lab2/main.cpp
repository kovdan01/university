#include <iostream>
#include <iomanip>
#include <cstddef>

using BYTE = std::int8_t;
using WORD = std::int16_t;

void lab(BYTE x, WORD y, WORD z)
{
    std::cout << std::hex << std::internal << std::setfill('0') << std::showbase
              << "x = " << std::setw(4) << (0xff & static_cast<int>(x)) << ", "
              << "y = " << std::setw(6) << y << ", "
              << "z = " << std::setw(6) << z << '\n';
    WORD v_c = (2 * x + (z + 4) * y) / (3 * x - 1) - 2;
    std::cout << std::hex;
    std::cout << "Result (C computed): " << std::setw(6) << v_c << '\n';

    WORD v_asm;
    asm(".intel_syntax           \n\t"
        "mov  ax, dx            ;\n\t" // z в AX
        "add  ax, 4             ;\n\t" // (z + 4) в AX
        "imul cx                ;\n\t" // (z + 4) * y в DX:AX
        "mov  cx, ax            ;\n\t" // (z + 4) * y в DX:CX
        "mov  al, bl            ;\n\t" // x в AL
        "mov  bx, dx            ;\n\t" // (z + 4) * y в BX:CX
        "cbw                    ;\n\t" // расширяем x до AX
        "sal  ax, 1             ;\n\t" // 2 * x в AX
        "cwd                    ;\n\t" // расширяем 2 * x до DX:AX
        "add  cx, ax            ;\n\t"
        "adc  bx, dx            ;\n\t" // 2 * x + (z + 4) * y в BX:CX
        "sar  ax, 1             ;\n\t" // x в AX
        "mov  dl, 3             ;\n\t" // 3 в dl
        "imul dl                ;\n\t" // 3 * x в AX
        "sub  ax, 1             ;\n\t" // (3 * x - 1) в AX
        "mov  dx, bx            ;\n\t" // 2 * x + (z + 4) * y в DX:CX
        "mov  bx, ax            ;\n\t" // (3 * x - 1) в BX
        "mov  ax, cx            ;\n\t" // 2 * x + (z + 4) * y в DX:AX
        "idiv bx                ;\n\t" // (2 * x + (z + 4) * y) / (3 * x - 1) в AX
        "sub  ax, 2             ;\n\t" // (2 * x + (z + 4) * y) / (3 * x - 1) - 2 в AX
        : "=a"(v_asm)
        : "b"(x), "c"(y), "d"(z)
    );
    std::cout << "Result (ASM computed): " << std::setw(6) << v_asm << '\n';
}

int main()
{
    BYTE x1, x2;
    WORD y1, y2, z1, z2, v1, v2;

    x1 =  0x01, y1 = -0x0002, z1 = -0x0006, v1 =  0x0001,
    x2 = -0x6b, y2 =  0x05f3, z2 =  0x0012, v2 = -0x0069;
    std::cout << std::hex << std::internal << std::setfill('0') << std::showbase;

    lab(x1, y1, z1);
    std::cout << "Expected result: " << std::setw(6) << v1 << "\n\n";
    lab(x2, y2, z2);
    std::cout << "Expected result: " << std::setw(6) << v2 << "\n\n";

    return 0;
}
