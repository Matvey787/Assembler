%include "MG_NASM32bit.inc"

section .data
    A db '1'

section .text
global _start
_start:
    ;PUTC [A]
    ;PUTC 10
    mov eax, 12345678
    SIGNINT eax
    PUTC 10
    PRINT "Number: "
    SIGNINT eax
    PUTC 10

    FINISH  
