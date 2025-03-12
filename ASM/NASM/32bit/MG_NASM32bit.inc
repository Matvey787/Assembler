%macro SIGNINT 1
    pusha
    section .data
        %%str times 11 db 0  ; буфер на 20 байт
        %%end db 0           ; нулевой терминатор
    section .text
        mov eax, %1          ; загрузить число
        mov ecx, eax
        shr ecx, 31
        lea edi, %%str + 10  ; указатель на конец буфера
        mov ebx, 10          ; делитель
        xor edx, edx

        ; Проверка на отрицательное число
        test ecx, ecx
        jz %%positive
        neg eax              ; преобразуем число в положительное
        %%positive:

        cmp eax, 0
        jne %%signint_loop
        mov byte [edi], '0'  ; случай для 0
        dec edi              ; сдвинуть edi перед '0'
        jmp %%signint_end

        %%signint_loop:
            cmp eax, 0
            je %%signint_end
            xor edx, edx
            div ebx
            add edx, '0'
            mov [edi], dl
            dec edi
            jmp %%signint_loop

        %%signint_end:
        test ecx, ecx
        jz %%positive2
        mov byte [edi], '-'  ; добавляем знак минуса
        dec edi              ; сдвигаем указатель на позицию перед минусом
        %%positive2:
        inc edi              ; сдвинуть edi на первую цифру
        PRINT edi
    popa
%endmacro

%macro PUTC 1
    push eax            ; <---|
    push ebx            ; <---| save all registers
    push ecx            ; <---|
    push edx            ; <---|

    %ifstr %1               ; check if it looks like PUTC 'A'
        section .data
            %%char db %1    ; tempurary initialization of char in data section
        section .text

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
        push dword %1        ; push the value to the stack
        mov eax, 4          ; sys_write
        mov ebx, 1          ; stdout
        mov ecx, esp        ; now ecx is storing the address of the value in stack
        mov edx, 1          ; length = 1 byte
        int 0x80            ; system interrupt
        pop edx             ; restore stack pointer (esp)
    %endif

    pop edx             ; <---|
    pop ecx             ; <---| restore all registers
    pop ebx             ; <---|
    pop eax             ; <---|
%endmacro

%macro PRINT 1
    %ifstr %1               ; Check if the argument is a string (e.g., PRINT "ABCD")
        section .data
            %%str db %1, 0  ; Storage for the string with a null terminator
        section .text
        mov esi, %%str      ; esi points to the string

    %else                   ; If the argument is not a string, assume it's an address
        mov esi, %1         ; esi points to the provided address
        
    %endif

    %%print_loop:           ; Unique label for the loop
        cmp byte [esi], 0   ; Check for the end of the string (null terminator)
        je %%print_end      ; Unique label for the end
        PUTC [esi]          ; Print the character at esi
        inc esi             ; Move to the next character
        jmp %%print_loop    ; Jump back to the loop
    %%print_end:            ; End of the loop
    xor esi, esi            ; Clear esi
%endmacro

%macro FINISH 0
    mov eax, 1              ; sys_exit
    xor ebx, ebx            ; exit code 0
    int 0x80                ; system interrupt
%endmacro

section .data
    minus_sign db '-'
    buffer db '00000000000', 0