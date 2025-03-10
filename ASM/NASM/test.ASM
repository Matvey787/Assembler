%include "matveyNASM.inc"

section .data
    A db '1'

section .text
global _start
_start:
    PUTC [A]
    PUTC 10

    ;mov eax, 10
    ;SIGNINT eax
    FINISH  
