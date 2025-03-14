%include "MG_NASM64bit.inc"

section .data
    A db "12345rfg"

section .text
global _start
_start:
    mov rax, 4
    lea r13, A
    ;PUTC 10
    ;mov rax, 123
    ;PRSIGN r12
    ;PRNUM r13
    PRBIN rax
    PUTC 10
    PRHEX rax
    PUTC 10
    ;PRINT "Number: "
    ; PRINTNUM rax
    ; PUTC 10

    FINISH