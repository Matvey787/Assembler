section .data
    A db '1', '2', '3', 0
    len equ $ - A - 1    

section .text
global _start
_start:

    ; Shift loop array by 1
    mov al, [A]     
    mov ecx, len - 1
    mov esi, 1      
shift_loop:
    mov bl, [A + esi]    
    mov [A + esi - 1], bl
    inc esi              
    loop shift_loop      
    mov [A + len - 1], al

    ; Print array
    mov ecx, len
    mov esi, 0          
print_loop:
    push ecx
    mov eax, 4        
    mov ebx, 1        
    lea ecx, [A + esi]
    mov edx, 1        
    int 0x80          
    inc esi           
    pop ecx
    loop print_loop

    ; Quit from prog
    mov eax, 1  
    xor ebx, ebx
    int 0x80    
