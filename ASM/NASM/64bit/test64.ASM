%include "MG_NASM64bit.inc"

section .data
    A db '1'

section .text
global _start
_start:
    ;PUTC [A]
    ;PUTC 10
    mov rax, 18446744073709551615
    PRINTNUM rax
    PUTC 10
    ; PRINT "Number: "
    ; PRINTNUM rax
    ; PUTC 10

    FINISH