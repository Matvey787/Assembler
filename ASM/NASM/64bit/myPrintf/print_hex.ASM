section .data
    symbols db "0123456789ABCDEF"
    

section .code

; rdi - buffer
print_hex:
    ; load number from stack
    mov rax, [rsp]
    mov edx, 4
    xor ecx, ecx

writeNum_loop:
    mov rbx, rax    ; number -> rbx
    and rbx, 0xF000 ; rbx & 11110000000000

    test rbx, rbx   ; rbx ? 0
    jz skip_zero

    test ecx, 0     ; 
    jz skip_zero

    mov ecx, 1
    lea rbx, [symbols + rbx]
    mov r12, [rbx]
    mov [rdi], r12
    inc rdi

skip_zero:

    shl rax, 4

    dec edx
    cmp edx, 0
    jae writeNum_loop

    ;cmp rax, 0
    ;jae writeNum_loop

    lea rbx, [buffer]

    ...








