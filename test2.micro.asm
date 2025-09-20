section .data
    format_int db '%d', 0

section .bss
    x resd 1
    y resd 1
    z resd 1

section .text
    global main
    extern scanf, printf

main:
    push rbp        ; Alinear stack a 16-bytes
    mov rbp, rsp

    mov rdi, format_int
    lea rsi, [x]
    xor eax, eax
    call scanf
    mov eax, 10
    push rax
    mov eax, [x]
    pop rbx
    add eax, ebx
    mov [x], eax
    mov rdi, format_int
    lea rsi, [y]
    xor eax, eax
    call scanf
    mov eax, 15
    push rax
    mov eax, [y]
    pop rbx
    add eax, ebx
    mov [y], eax
    mov eax, [x]
    push rax
    mov eax, [y]
    pop rbx
    add eax, ebx
    mov [z], eax
    mov eax, [z]
    mov rdi, format_int
    mov esi, eax
    xor eax, eax
    call printf

    pop rbp         ; Restaurar stack
    mov eax, 0
    ret
