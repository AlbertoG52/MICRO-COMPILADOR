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
    mov edi, format_int
    lea rsi, [x]
    xor eax, eax
    call scanf
    mov eax, 10
    mov [x], eax
    mov edi, format_int
    lea rsi, [y]
    xor eax, eax
    call scanf
    mov eax, 15
    mov [y], eax
    mov edi, format_int
    lea rsi, [z]
    xor eax, eax
    call scanf
    mov eax, [x]
    push rax
    mov eax, [y]
    pop rbx
    add eax, ebx
    mov [z], eax
    mov eax, [z]
    mov edi, format_int
    mov esi, eax
    xor eax, eax
    call printf
