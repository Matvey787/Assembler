%macro SIGNINT 1
    section .data
        %%str db 20 dup(0), 0  ; storage for number with null terminator
    section .text

    %ifnum %1
        lea eax, %1
        mov ebx, 10
        signint_loop:
        cmp eax, 0
        je signint_end
        idiv ebx
        add edx, '0'
        shl [eax], 8
        mov [eax], dl
        jmp signint_loop
        signint_end:
    %endif
        PRINT %%str
%endmacro

%macro PUTC 1
    %ifstr %1               ; check if it looks like PUTC 'A'
        section .data
            %%char db %1    ; tempurary initialization of char in data section
        section .text

        push eax            ; <---|
        push ebx            ; <---| save all registers
        push ecx            ; <---|
        push edx            ; <---|

        mov eax, 4          ; sys_write
        mov ebx, 1          ; stdout
        mov ecx, %%char     ; char to write
        mov edx, 1          ; length = 1 byte
        int 0x80            ; system interrupt

    %elifid %1              ; true if A is the name of a label or variable (for example PUTC A)
        mov eax, 4          ; sys_write
        mov ebx, 1          ; stdout
        mov ecx, %1         ; char to write
        mov edx, 1          ; length = 1 byte
        int 0x80            ; system interrupt

    %else                   ; check if it looks like PUTC [A]
        push word %1        ; push the value to the stack
        mov eax, 4          ; sys_write
        mov ebx, 1          ; stdout
        mov ecx, esp        ; now ecx is storing the address of the value in stack
        mov edx, 1          ; length = 1 byte
        int 0x80            ; system interrupt
        pop eax             ; restore stack pointer (esp)
    %endif
        pop ax              ; <---| 
        pop bx              ; <---| restore all registers
        pop cx              ; <---|
        pop dx              ; <---|
%endmacro

%macro PRINT 1
    %ifstr %1               ; check if it looks like PRINT "ABCD"
        section .data
            %%str db %1, 0  ; storage for string with null terminator
        section .text
        mov esi, %%str      ; esi is a pointer to the string

        print_loop:
        cmp byte [esi], 0   ; check the end of str
        je print_end
        PUTC [esi]          ; print symbol from esi
        inc esi             ; add 1 to esi
        jmp print_loop
        print_end:
    %endif
    xor esi, esi
%endmacro

%macro FINISH 0
    mov eax, 1              ; sys_exit
    xor ebx, ebx            ; exit code 0
    int 0x80                ; system interrupt
%endmacro

section .data
    minus_sign db '-'
    buffer db '00000000000', 0