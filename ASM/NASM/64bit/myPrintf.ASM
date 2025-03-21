; printf (c) Matvey Galicyn

%include "MG_NASM64bit.inc"

section .data
    buffer db 500 dup(0)
    EOF equ -1
    symbols db "0123456789ABCDEF"



section .text
global myprintf
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
;       Defined globally and does not change anywhere:
;       rdi - format string
;       rsi - buffer
;       rbp - stack frame
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

; |---------------------------------------------------------------------------------------|
; | A proc that recursively translates a number into ascii code and stores it in a buffer |
; | Entry: rax - number to be translated,                                                 |
; |        rbx - divisor,                                                                 |
; |        rsi - buffer                                                                   |
; | Exit:  number in buffer                                                               |
; | Destr: RAX, RBX, RDX                                                                  |
; |---------------------------------------------------------------------------------------|
; print_num_recursive:
;     push rbp
;     mov rbp, rsp
;     cmp rax, 0
;     jz .rec_done
;     xor rdx, rdx
;     div rbx
;     cmp rdx, 10
;     jge .addA
;     add rdx, '0'
;     jmp .skip
; .addA:
;     add rdx, 'A' - 10
; .skip:
;     push rdx
;     call print_num_recursive
;     pop rdx
;     mov [rsi], dl
;     inc rsi
; .rec_done:
;     mov rsp, rbp
;     pop rbp
;     ret

; |-----------------------------------------------------|
; | A proc that process %d (decimal) from format string |
; | Entry: rbp - stack frame                            |
; |        r8 - offset of stack frame                   |
; |        rsi - buffer                                 |
; | Exit:  number in buffer                             |
; | Destr: RAX, RBX, RCX, RDX, r10                      |
; |-----------------------------------------------------|
print_decimal:
    mov rax, [rsp]
    
    mov rcx, rax
    shr rcx, 63

    test rax, rcx
    jge .positive_start
    neg rax
    mov byte [rsi], '-'
    inc rsi
.positive_start:
    mov r10, rsi

    mov rbx, 10
.convert_loop:
    cmp rax, 0
    je .convert_done
    xor rdx, rdx
    div rbx
    add rdx, '0'
    mov [rsi], dl
    inc rsi
    jmp .convert_loop
.convert_done:
    mov rcx, r10
    mov r10, rsi
    dec rsi

.reverse_loop:
    cmp rcx, rsi
    jge .reverse_done
    mov al, byte [rcx]
    mov ah, byte [rsi]
    mov byte [rsi], al
    mov byte [rcx], ah
    inc rcx
    dec rsi
    jmp .reverse_loop
.reverse_done:

    mov rsi, r10

    inc rdi

    add rsp, 8
    cmp rsp, rbp
    jne .double_add
    add rsp, 16
.double_add:

    jmp myprintf.process_format_string

; |--------------------------------------------------|
; | A proc that process %% from format string        |
; | Entry: rbp - stack frame,                        |
; |        r8 - offset of stack frame,               |
; |        rsi - buffer,                             |
; |        rdi - format string                       |
; | Exit: % in buffer                                |
; | Destr: RAX, RBX, RCX, RDX, r10                   |
; |--------------------------------------------------|
print_percent:
    mov rax, [rdi]
    mov [rsi], rax
    inc rdi
    inc rsi
    jmp myprintf.process_format_string

; |-----------------------------------------------------------------------------------|
; | A proc that process %x (hexadecimal) from format string |
; | Entry: rbp - stack frame,                                                         |
; |        r8 - offset of stack frame,                                                |
; |        rsi - buffer,                                                              |
; |        rdi - format string                                                        |
; |        rbx - divisor of system                                                    |
; | Exit:  number in buffer                                                           |
; | Destr: RAX, RBX, RCX, RDX, r10                                                    |
; |-----------------------------------------------------------------------------------|

print_hex:
    ; load number from stack
    mov rax, [rsp]
    mov edx, 8
    xor ecx, ecx

writeNum_loop:
    mov rbx, rax    ; number -> rbx
    
    convert_loop:
    and rbx, 0xF000
    or rcx, rbx
    test rcx, rcx
    jz skip_adding_to_buff

    lea rbx, [symbols + rbx]    
    mov [rsi], rbx
    inc rsi
    inc rcx
skip_adding_to_buff:

    shl rax, 4
    dec edx
    cmp edx, 0
    jne writeNum_loop


    ;inc rsi
    add rsp, 8
    cmp rsp, rbp
    jne .double_add
    add rsp, 16
.double_add:

    jmp myprintf.process_format_string

; print_hex:
;     ; load number from stack
;     mov rbx, 16
;     mov rax, [rsp]
;     mov r10, rsi

; .convert_loop:
;     cmp rax, 0
;     je .convert_done
;     xor rdx, rdx
;     div 
;     add rdx, 'A'
;     mov [rsi], dl
;     inc rsi
;     jmp .convert_loop
; .convert_done:

;     mov rcx, r10
;     mov r10, rsi
;     dec rsi

; .reverse_loop:
;     cmp rcx, rsi
;     jge .reverse_done
;     mov al, byte [rcx]
;     mov ah, byte [rsi]
;     mov byte [rsi], al
;     mov byte [rcx], ah
;     inc rcx
;     dec rsi
;     jmp .reverse_loop
; .reverse_done:
;     mov rsi, r10

;     ;xor rdx, rdx
;     ; call print_num_recursive

;     inc rdi
    
;     add rsp, 8
;     cmp rsp, rbp
;     jne .double_add
;     add rsp, 16
; .double_add:

;     jmp myprintf.process_format_string

; |-----------------------------------------------------------------------------------|
; | A proc that process %b (binary), %o (occtal) from format string                   |
; | Entry: rbp - stack frame,                                                         |
; |        rsi - buffer,                                                              |
; |        rdi - format string                                                        |
; |        rbx - divisor of system                                                    |
; | Exit:  number in buffer                                                           |
; | Destr: RAX, RBX, RCX, RDX, r10                                                    |
; |-----------------------------------------------------------------------------------|

print_bin_oct:
    ; load number
    mov rax, [rsp]
    mov r10, rsi

.convert_loop:
    cmp rax, 0
    je .convert_done
    xor rdx, rdx
    div rbx
    add rdx, '0'
    mov [rsi], dl
    inc rsi
    jmp .convert_loop
.convert_done:
    mov rcx, r10
    mov r10, rsi
    dec rsi

    ; reverse number
.reverse_loop:
    cmp rcx, rsi
    jge .reverse_done

    mov al, byte [rcx]
    mov ah, byte [rsi]
    mov byte [rsi], al
    mov byte [rcx], ah
    inc rcx
    dec rsi
    jmp .reverse_loop
.reverse_done:

    mov rsi, r10
    ;xor rdx, rdx
    ; call print_num_recursive
    inc rdi
    
    add rsp, 8
    cmp rsp, rbp
    jne .double_add
    add rsp, 16
.double_add:

    jmp myprintf.process_format_string

; |--------------------------------------------------|
; | A proc that process %c (char) from format string |
; | Entry: rbp - stack frame,                        |
; |        r8 - offset of stack frame,               |
; |        rsi - buffer,                             |
; |        rdi - format string                       |
; | Exit:  number in buffer                          |
; | Destr: RAX, RBX, RCX, RDX                        |
; |--------------------------------------------------|
print_char:
    mov rax, [rsp]
    mov [rsi], rax
    inc rsi
    inc rdi

    add rsp, 8
    cmp rsp, rbp
    jne .double_add
    add rsp, 16
.double_add:

    jmp myprintf.process_format_string

; |----------------------------------------------------|
; | A proc that process %s (string) from format string |
; | Entry: rbp - stack frame,                          |
; |        rsi - buffer,                               |
; |        rdi - format string                         |
; | Exit:  number in buffer                            |
; | Destr: RAX, RBX, RCX, RDX                          |
; |----------------------------------------------------|
print_string:
    mov rax, [rsp]

.wr_string:
    movzx rcx, byte [rax]  
    test cl, cl
    je .done
    mov [rsi], cl
    inc rsi
    inc rax
    jmp .wr_string

.done:
    inc rdi
    add rsp, 8
    cmp rsp, rbp
    jne .double_add
    add rsp, 16
.double_add:

    jmp myprintf.process_format_string


; |-----------------------------------------------------------------|
; | A proc printing a format string with specifiers                 |
; | Entry: rdi - format string,                                     |
; |        rsi - buffer,                                            |
; |        rsi, rdx, rcx, r8, r9 - args of frmt str,                |
; |        stack - args of frmt str (optionally if num of args > 6) |
; | Exit:  string in bash                                           |
; | Destr: RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP                   |
; |-----------------------------------------------------------------|
myprintf:
    mov r11, rbp
    push rbp                    ; save old base pointer of stack frame
    mov rbp, rsp                ; set new pointer of stack frame

    push r9
    push r8
    push rcx
    push rdx
    push rsi
    lea rsi, buffer
    lea r8, buffer

.process_format_string:
    movzx rax, byte [rdi]       ; load char byte from format string
    cmp rax, 0
    jz .done

    cmp rax, '%'
    je .handle_specifier

    mov [rsi], rax

    inc rdi
    inc rsi
    jmp .process_format_string

.handle_specifier:
    inc rdi                     ; skip '%'
    movzx rax, byte [rdi]       ; load specifier
    sub rax, '%'
    lea rax, [jmp_table + rax * 8]
    jmp [rax]
    
.set_bin:
    mov rbx, 2
    jmp print_bin_oct

.set_oct:
    mov rbx, 8
    jmp print_bin_oct

; ------------------------------ myprintf.invalid_specifier ------------------------------
; If specifier is invalid, print error message and return EOF
.invalid_specifier:
    mov rsp, rbp
    pop rbp
    mov rax, EOF
    ret
; ----------------------------------------------------------------------------------------


; ----------------------------------- myprintf.done -----------------------------------
; If everythink is ok, print buffer to bash and return number of chars printed
.done:
    mov rsp, rbp
    pop rbp

    mov r8, rsi ; save old rsi of buffer

    ; print buffer to bash
    mov rax, 1
    mov rdi, 1
    mov rdx, rsi
    lea rsi, [buffer]
    sub rdx, rsi
    syscall

    ; return value
    mov rax, r8
    sub rax, rsi
    
    ret
; -------------------------------------------------------------------------------------

section .data
    jmp_table:
        dq print_percent
        times 'b' - '%' - 1 dq myprintf.invalid_specifier
        dq myprintf.set_bin
        dq print_char
        dq print_decimal
        times 'o' - 'd' - 1 dq myprintf.invalid_specifier
        dq myprintf.set_oct
        times 's' - 'o' - 1 dq myprintf.invalid_specifier
        dq print_string
        times 'x' - 's' - 1 dq myprintf.invalid_specifier
        dq print_hex
        times 12 dq myprintf.invalid_specifier
