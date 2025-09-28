section .data
    format_int db '%d', 0

    format_int_newline db '%d', 10, 0

section .bss
    x resd 1
    y resd 1
    z resd 1

section .text
    global main
    extern scanf, printf

main:
    push rbp
    mov rbp, rsp

    mov rdi, format_int
    lea rsi, [x]
    xor eax, eax
    call scanf
    mov eax, [x]
    push rax
    mov eax, [z]
    pop rbx
    add eax, ebx
    mov [y], eax
    mov eax, [y]
    mov rdi, format_int_newline

    mov esi, eax
    xor eax, eax
    call printf

    pop rbp         ; Restaurar stack
    mov eax, 0
    ret
