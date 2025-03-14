; MG_NASM64bit.inc - A library of NASM macros for 64-bit assembly programming
; Copyright (c) 2025 Matvey Galicyn
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES.
; THE AUTHORS OR COPYRIGHT HOLDERS ARE NOT RESPONSIBLE FOR ANY PROBLEMS,
; DAMAGES, OR CLAIMS RELATED TO THE USE OF THIS SOFTWARE. 

%macro PRNUM 1
    multiPush rax, rbx, rcx, rdx, rsi, rdi

    section .data
        %%str times 20 db 0     ; buffer for unsigned number
        %%end db 0              ; null terminator
    section .text
        mov rax, %1             ; load number in rax
        mov rcx, rax
        shr rcx, 63             ; need for checking sign
        lea rdi, [%%str + 19]
        mov rbx, 10             ; divisor
        xor rdx, rdx

        test rcx, rcx           ; check sign
        jz %%positive
        neg rax                 ; now it's positive number
        %%positive:

        cmp rax, 0              ; check if number is zero
        jne %%signint_loop
        mov byte [rdi], '0'  
        dec rdi
        jmp %%signint_end

        %%signint_loop:
            cmp rax, 0
            je %%signint_end
            xor rdx, rdx
            div rbx             ; divide rax on rbx (result in rax, remainder in rdx)
            add rdx, '0'        ; digit -> number
            mov [rdi], dl       ; write ascii digit
            dec rdi
            jmp %%signint_loop

        %%signint_end:
        test rcx, rcx
        jz %%positive2
        mov byte [rdi], '-'  ; add sign
        dec rdi

        %%positive2:
        inc rdi              ; move to first digit
        PRINT rdi

    multiPop rdi, rsi, rdx, rcx, rbx, rax
%endmacro

%macro PRBIN 1
    multiPush rax, rbx, rcx, rdx, rsi, rdi
    section .data
        %%str times 64 db 0  ; buffer for binary number
        %%end db 0           ; zero terminator
    section .text
    ; next code same as in PRNUM
    mov rax, %1          
    lea rdi, [%%str + 63]
    mov rbx, 2

    %%bin_loop:
        cmp rax, 0
        jle %%bin_end
        xor rdx, rdx
        div rbx      
        add rdx, '0' 
        mov [rdi], dl
        dec rdi
        jmp %%bin_loop
    %%bin_end:
    mov [rdi], byte 'b'
    dec rdi
    mov [rdi], byte '0'            
    PRINT rdi
    multiPop rdi, rsi, rdx, rcx, rbx, rax
%endmacro

%macro PRHEX 1
    multiPush rax, rbx, rcx, rdx, rsi, rdi
    section .data
        %%str times 18 db 0  ; buffer for hex number
        %%end db 0           ; zero terminator
    section .text
    ; next code same as in PRNUM
    mov rax, %1          
    lea rdi, [%%str + 15]
    mov rbx, 16

    %%hex_loop:
        cmp rax, 0
        jle %%hex_end
        xor rdx, rdx
        div rbx      
        add rdx, '0' 
        mov [rdi], dl
        dec rdi
        jmp %%hex_loop
    %%hex_end:
    mov [rdi], byte 'x'
    dec rdi
    mov [rdi], byte '0'            
    PRINT rdi
    multiPop rdi, rsi, rdx, rcx, rbx, rax
%endmacro
        
%macro PUTC 1
    multiPush rax, rbx, rcx, rdx, rsi, rdi

    %ifstr %1
        section .data
            %%char db %1
        section .text
            mov rax, 1          ; sys_write
            mov rdi, 1          ; stdout (file descriptor)
            lea rsi, [%%char]   ; char address
            mov rdx, 1          ; length = 1
            syscall             ; system call

    %elifid %1                  ; if it's identifier
            mov rax, 1
            mov rdi, 1
            mov rsi, byte %1         
            mov rdx, 1
            syscall
    %else
        push qword %1
        mov rax, 1
        mov rdi, 1
        mov rsi, rsp        
        mov rdx, 1
        syscall
        pop rdx             
    %endif

    multiPop rdi, rsi, rdx, rcx, rbx, rax
%endmacro

%macro PRINT 1
    %ifstr %1
        section .data
            %%str db %1, 0
        section .text
            lea rsi, [%%str]    ; rsi is pointing to the created string
    %else
            mov rsi, %1         ; rsi is pointing to the passed string
    %endif

    %%print_loop:
        cmp byte [rsi], 0       ; check the end of the string
        je %%print_end
        PUTC [rsi]              ; print the current character
        inc rsi
        jmp %%print_loop
    %%print_end:
        xor rsi, rsi            ; clear rsi
%endmacro

%macro FINISH 0
    mov rax, 60                 ; sys_exit
    xor rdi, rdi                ; return code 0
    syscall                     ; system call
%endmacro

%macro multiPush 1-*
    %rep %0
        push %1
        %rotate 1
    %endrep
%endmacro

%macro multiPop 1-*
    %rep %0
        pop %1
        %rotate 1
    %endrep
%endmacro
