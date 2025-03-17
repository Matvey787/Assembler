; printf (c) Matvey Galicyn

%include "MG_NASM64bit.inc"

section .data
    buffer db 500 dup(0)
    
section .text
global myprintf
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;       Defined globally and does not change anywhere:
;       rdi - format string
;       rsi - buffer
;       rbp - stack frame
;       r8 - offset of stack frame
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

; |---------------------------------------------------------------------------------------|
; | A proc that recursively translates a number into ascii code and stores it in a buffer |
; | Entry: rax - number to be translated,                                                 |
; |        rbx - divisor,                                                                 |
; |        rsi - buffer                                                                   |
; | Exit:  number in buffer                                                               |
; | Destr: RAX, RBX, RDX                                                                  |
; |---------------------------------------------------------------------------------------|
print_num_recursive:
    push rbp
    mov rbp, rsp
    cmp rax, 0
    jz .rec_done
    xor rdx, rdx
    div rbx
    cmp rdx, 10
    jge .addA
    add rdx, '0'
    jmp .skip
.addA:
    add rdx, 'A' - 10
.skip:
    push rdx
    call print_num_recursive
    pop rdx
    mov [rsi], dl
    inc rsi
.rec_done:
    mov rsp, rbp
    pop rbp
    ret

; |-----------------------------------------------------|
; | A proc that process %d (decimal) from format string |
; | Entry: rbp - stack frame                            |
; |        r8 - offset of stack frame                   |
; |        rsi - buffer                                 |
; | Exit:  number in buffer                             |
; | Destr: RAX, RBX, RCX, RDX, r10                      |
; |-----------------------------------------------------|
print_decimal:
    mov r10, rbp
    add r10, r8
    mov rax, [r10]
    mov rcx, rax
    shr rcx, 63
    mov rbx, 10
    xor rdx, rdx
    test rcx, rcx
    jz .positive_start
    neg rax
    mov byte [rsi], '-'
    inc rsi
.positive_start:
    test rax, rax
    jnz .convert_loop
    mov byte [rsi], '0'
    inc rsi
    jmp .decimal_done
.convert_loop:
    call print_num_recursive
.decimal_done:
    inc rdi
    add r8, r9
    cmp r8, -48
    jne .skip_stack_reverse
    mov r8, 16
    mov r9, 8
.skip_stack_reverse:
    jmp myprintf.process_format_string

; |-----------------------------------------------------------------------------------|
; | A proc that process %b (binary), %o (occtal), %x (hexadecimal) from format string |
; | Entry: rbp - stack frame,                                                         |
; |        r8 - offset of stack frame,                                                |
; |        rsi - buffer,                                                              |
; |        rdi - format string                                                        |
; |        rbx - divisor of system                                                    |
; | Exit:  number in buffer                                                           |
; | Destr: RAX, RBX, RCX, RDX, r10                                                    |
; |-----------------------------------------------------------------------------------|
print_bin_oct_hex:
    mov r10, rbp
    add r10, r8
    mov rax, [r10]
    mov rcx, rax
    shr rcx, 63
    xor rdx, rdx
    call print_num_recursive
    inc rdi
    add r8, r9
    cmp r8, -48
    jne .skip_stack_reverse
    mov r8, 16
    mov r9, 8
.skip_stack_reverse:
    jmp myprintf.process_format_string

; |--------------------------------------------------|
; | A proc that process %c (char) from format string |
; | Entry: rbp - stack frame,                        |
; |        r8 - offset of stack frame,               |
; |        rsi - buffer,                             |
; |        rdi - format string                       |
; | Exit:  number in buffer                          |
; | Destr: RAX, RBX, RCX, RDX, r10                   |
; |--------------------------------------------------|
print_char:
    mov r10, rbp
    add r10, r8
    mov rax, [r10]
    mov [rsi], rax
    inc rsi
    inc rdi
    add r8, r9
    cmp r8, -48
    jne .skip_stack_reverse
    mov r8, 16
    mov r9, 8
.skip_stack_reverse:
    jmp myprintf.process_format_string

; |----------------------------------------------------|
; | A proc that process %s (string) from format string |
; | Entry: rbp - stack frame,                          |
; |        r8 - offset of stack frame,                 |
; |        rsi - buffer,                               |
; |        rdi - format string                         |
; | Exit:  number in buffer                            |
; | Destr: RAX, RBX, RCX, RDX, r10                     |
; |----------------------------------------------------|
print_string:
    mov rax, rbp

.wr_string:
    add rax, rbp
    movzx rcx, byte [rax]  
    test cl, cl
    je .done
    mov [rsi], cl
    inc rsi
    add rbp, r9
    jmp .wr_string

.done:
    inc rdi
    add r8, r9
    cmp r8, -48
    jne .skip_stack_reverse
    mov r8, 16
    mov r9, 8
.skip_stack_reverse:
    jmp myprintf.process_format_string


; |-----------------------------------------------------------------|
; | A proc printing a format string with specifiers                 |
; | Entry: rdi - format string,                                     |
; |        rsi - buffer,                                            |
; |        rsi, rdx, rcx, r8, r9 - args of frmt str,                |
; |        stack - args of frmt str (optionally if num of args > 6) |
; | Exit:  string in bash                                           |
; | Destr: RAX, RBX, RCX, RDX, RSI, RDI, R8, R9, RBP, RSP           |
; |-----------------------------------------------------------------|
myprintf:
    push rbp                    ; save old base pointer of stack frame
    mov rbp, rsp                ; set new pointer of stack frame

    push rsi
    push rdx
    push rcx
    push r8
    push r9

    lea rsi, buffer
    mov r8, -8
    mov r9, -8

.process_format_string:
    movzx rax, byte [rdi]       ; load char byte from format string
    cmp rax, 0
    jz .done

    cmp rax, '%'
    je .handle_specifier

    cmp rax, '\'
    je .handle_special_char

    mov [rsi], rax

    ; PUTC [rbx]                  ; print char

    inc rdi
    inc rsi
    jmp .process_format_string

; for \n construction
.handle_special_char:
    inc rdi                     ; skip '\'
    movzx rax, byte [rdi]
    cmp rax, 'n'
    jne .not_slash_n
    mov [rsi], byte 10
.not_slash_n:
    mov [rsi], rax
    inc rsi
    jmp .process_format_string



.handle_specifier:
    inc rdi                     ; skip '%'
    movzx eax, byte [rdi]       ; load specifier

    ; checking of specifiers
    cmp eax, 'd'
    je .set_decimal
    cmp eax, 's'
    je .set_string
    cmp eax, 'c'
    je .set_char
    cmp eax, 'b'
    je .set_bin
    cmp eax, 'o'
    je .set_oct
    cmp eax, 'x'
    je .set_hex

    jmp .invalid_specifier      ; handle invalid specifier

.set_decimal:
    mov rax, 0                  ; %d -> index = 0
    jmp .do_jump

; for hex, bin, obt index = 1
.set_bin:
    mov rax, 1
    mov rbx, 2
    jmp .do_jump

.set_oct:
    mov rax, 1
    mov rbx, 8
    jmp .do_jump

.set_hex:
    mov rax, 1
    mov rbx, 16
    jmp .do_jump


.set_char:
    mov rax, 2                  ; %c -> index 2
    jmp .do_jump

.set_string:
    mov rax, 3                  ; %s -> index 3

.do_jump:
    lea rax, [jmp_table + rax * 8]
    jmp [rax]

.invalid_specifier:
    inc rbx                     ; skip invalid specifier
    jmp .process_format_string

; ---------------------------------------------------------------------------

.done:
    mov rsp, rbp
    pop rbp

    ; print buffer to bash
    mov rax, 1
    mov rdi, 1
    mov rdx, rsi
    lea rsi, [buffer]
    sub rdx, rsi
    syscall

    ret

section .data
    jmp_table:
        dq print_decimal
        dq print_bin_oct_hex
        dq print_char
        dq print_string
