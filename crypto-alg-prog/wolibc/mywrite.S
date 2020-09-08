.globl mywrite

.section .text

mywrite:
    mov $1, %rax // 1 <-> write
    syscall // call write
            // rdi, rsi and rdx already contain parameters
            // fd, buf and count, passed to mywrite
    ret
