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


%macro PRINTNUM 1
    multiPush rax, rbx, rcx, rdx, rsi, rdi

    section .data
        %%str times 20 db 0  ; Буфер на 11 байт
        %%end db 0           ; Нулевой терминатор
    section .text
        mov rax, %1          ; Загрузить число в rax (64-битный регистр)
        mov rcx, rax
        shr rcx, 63          ; Проверка знака (64-битный сдвиг)
        lea rdi, [%%str + 19]; Указатель на конец буфера
        mov rbx, 10          ; Делитель
        xor rdx, rdx         ; Обнулить rdx перед делением

        ; Проверка на отрицательное число
        test rcx, rcx
        jz %%positive
        neg rax              ; Преобразуем в положительное
        %%positive:

        cmp rax, 0
        jne %%signint_loop
        mov byte [rdi], '0'  ; Случай для 0
        dec rdi
        jmp %%signint_end

        %%signint_loop:
            cmp rax, 0
            je %%signint_end
            xor rdx, rdx
            div rbx          ; Деление rax на rbx (результат в rax, остаток в rdx)
            add rdx, '0'     ; Преобразовать остаток в ASCII
            mov [rdi], dl    ; Записать символ
            dec rdi
            jmp %%signint_loop

        %%signint_end:
        test rcx, rcx
        jz %%positive2
        mov byte [rdi], '-'  ; Добавить знак минуса
        dec rdi
        %%positive2:
        inc rdi              ; Сдвинуть rdi на первую цифру
        PRINT rdi
    ; pop rdi
    ; pop rsi
    ; pop rdx
    ; pop rcx
    ; pop rbx
    ; pop rax

    multiPop rdi, rsi, rdx, rcx, rbx, rax
%endmacro

%macro PUTC 1
    multiPush rax, rbx, rcx, rdx, rsi, rdi

    %ifstr %1
        section .data
            %%char db %1
        section .text
            mov rax, 1          ; sys_write
            mov rdi, 1          ; stdout (файловый дескриптор 1)
            lea rsi, [%%char]   ; Адрес символа
            mov rdx, 1          ; Длина = 1 байт
            syscall             ; Системный вызов
    %elifid %1
            mov rax, 1
            mov rdi, 1
            mov rsi, %1         ; Адрес идентификатора
            mov rdx, 1
            syscall
    %else
        push qword %1       ; Используем qword для 64-битной архитектуры
        mov rax, 1
        mov rdi, 1
        mov rsi, rsp        ; Адрес на стеке
        mov rdx, 1
        syscall
        pop rdx             ; Очистить стек
    %endif
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    ; multiPop rdi, rsi, rdx, rcx, rbx, rax
%endmacro

%macro PRINT 1
    %ifstr %1
        section .data
            %%str db %1, 0
        section .text
            lea rsi, [%%str]    ; rsi указывает на строку
    %else
            mov rsi, %1         ; rsi указывает на переданный адрес
    %endif

    %%print_loop:
        cmp byte [rsi], 0   ; Проверка на конец строки
        je %%print_end
        PUTC [rsi]          ; Вывод символа
        inc rsi
        jmp %%print_loop
    %%print_end:
        xor rsi, rsi        ; Очистить rsi
%endmacro

%macro FINISH 0
    mov rax, 60             ; sys_exit
    xor rdi, rdi            ; Код возврата 0
    syscall                 ; Системный вызов
%endmacro

%macro multiPush 1-*
    %rep %0
        push %1
        %rotate 1
    %endrep
%endmacro

%macro multiPop 1-*
    %rep %0
        %rotate 1
        pop %1
    %endrep
%endmacro
