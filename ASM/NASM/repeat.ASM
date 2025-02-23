section .data
    prompt db 'Введите строку: ', 0 ; 
    prompt_len equ $ - prompt

    char_prompt db 'Введите символ: ', 0
    char_prompt_len equ $ - char_prompt

    output db 'Вы ввели строку: ', 0
    output_len equ $ - output

    char_output db 'Вы ввели символ: ', 0
    char_output_len equ $ - char_output

section .bss
    input_buffer resb 100
    input_len resb 1
    char_buffer resb 1

section .text
    global _start

_start:
    
    mov eax, 4              ; sys_write
    mov ebx, 1              ; file descriptor (stdout)
    mov ecx, prompt         ; pointer to str
    mov edx, prompt_len     ; len of str
    int 0x80

    mov eax, 3              ; sys_read
    mov ebx, 0              ; file descriptor (stdin)
    mov ecx, input_buffer   ; pointer to str
    mov edx, 100            ; len of str
    int 0x80

    ; Save len of readed str
    mov [input_len], al

    mov eax, 4          
    mov ebx, 1          
    mov ecx, char_prompt
    mov edx, char_prompt_len
    int 0x80

    
    mov eax, 3          
    mov ebx, 0          
    mov ecx, char_buffer 
    mov edx, 1          
    int 0x80

    
    mov eax, 4          
    mov ebx, 1          
    mov ecx, output      
    mov edx, output_len  
    int 0x80

    ; print input string
    mov eax, 4          
    mov ebx, 1          
    mov ecx, input_buffer 
    movzx edx, byte [input_len] ; from op mem to reg
    int 0x80

    
    mov eax, 4          
    mov ebx, 1          
    mov ecx, char_output 
    mov edx, char_output_len 
    int 0x80

    ; print char
    mov eax, 4          
    mov ebx, 1          
    mov ecx, char_buffer 
    mov edx, 1          
    int 0x80

    
    mov eax, 1          
    xor ebx, ebx        
    int 0x80