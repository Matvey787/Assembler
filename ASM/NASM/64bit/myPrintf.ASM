; printf (c) Matvey Galicyn

%include "MG_NASM64bit.inc"

section .text
global myprintf         

; rdi - first argument (format string)
; rsi - second argument (decimal)
; rdx - third argument (string)
; rcx - fourth argument (char)
; r8  - fifth argument (binary)
; r9  - sixth argument (hexadecimal)

myprintf:
    push rbp                    ; save old base pointer of stack frame
    mov rbp, rsp                ; set new pointer of stack frame

    ; callee-saved registers
    push rbx                    ; save rbx
    push r12                    ; save r12
    push r13                    ; save r13
    
    mov rbx, rdi                ; rbx is pointing to format string
    mov r12, rsi                ; argument of %d
    mov r13, rdx                ; argument of %s

.process_format_string:
    movzx eax, byte [rbx]       ; load char byte from format string
    test eax, eax               ; check if it is null terminator
    jz .done

    cmp eax, '%'                ; check if it is a format specifier
    je .handle_specifier        ; handle chars after %

    PUTC [rbx]                  ; print char

    inc rbx
    jmp .process_format_string

.handle_specifier:
    inc rbx                     ; skip '%'
    movzx eax, byte [rbx]       ; load specifier

    cmp eax, 'd'                ; check if it is a decimal specifier
    je .set_decimal
    cmp eax, 's'                ; check if it is a string specifier
    je .set_string
    cmp eax, 'c'                ; check if it is a char specifier
    je .set_char
    cmp eax, 'b'                ; check if it is a binary specifier
    je .set_bin
    cmp eax, 'h'                ; check if it is a hexadecimal specifier
    je .set_hex

    jmp .invalid_specifier      ; handle invalid specifier

.set_decimal:
    mov rax, 0                  ; %d -> index 0
    jmp .do_jump

.set_bin:
    mov rax, 1                  ; %b -> index 1
    jmp .do_jump

.set_hex:
    mov rax, 2                  ; %h -> index 2
    jmp .do_jump

.set_char:
    mov rax, 3                  ; %c -> index 3
    jmp .do_jump

.set_string:
    mov rax, 4                  ; %s -> index 4

.do_jump:
    lea rax, [jmp_table + rax * 8]
    jmp [rax]

; -------------------- funcs for printing specifiers args --------------------
.print_decimal:
    PRNUM r12
    inc rbx
    jmp .process_format_string

.print_binary:
    PRBIN r12
    inc rbx
    jmp .process_format_string

.print_hexadecimal:
    PRHEX r12
    inc rbx
    jmp .process_format_string

.print_char:
    PUTC [r13]
    inc rbx
    jmp .process_format_string

.print_string:
    PRINT [r13]
    inc rbx
    jmp .process_format_string

.invalid_specifier:
    inc rbx                     ; skip invalid specifier
    jmp .process_format_string

; ---------------------------------------------------------------------------

.done:
    pop r13
    pop r12
    pop rbx
    mov rsp, rbp
    pop rbp
    ret

section .data
    jmp_table:
        dq myprintf.print_decimal
        dq myprintf.print_binary
        dq myprintf.print_hexadecimal
        dq myprintf.print_char
        dq myprintf.print_string
