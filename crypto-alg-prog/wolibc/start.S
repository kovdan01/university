.globl _start

_start:
    call main
    mov  $60, %rax  // 60 <-> exit
    xor  %rdi, %rdi // exit code 0
    syscall         // call exit
