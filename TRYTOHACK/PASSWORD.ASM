.model tiny 
.data
    askPasswordMSG db 'Enter the password: ', 0Ah, '$'
    correctPassword db 'DED32'          ; Эталонный пароль
    winner db 'You have received admin rights!$'
    looser db 'Are you a real hacker? Looooooser... Ahahha!', 0Ah, 'Incorrect password!$'
    lenOfcorrectPassword db 5           ; Длина эталонного пароля
    hint db 'Hint: behind |000001F5 33DB xor bx,bx|  and  |00000220 50 push ax|  29 instructions$'
.code
org 100h 

Start:
    mov ax, cs
    mov ds, ax                      ; DS = CS
    mov es, ax                      ; ES = CS (для cmpsb)
    jmp Empty2
    trash db 200 dup(0)            ; Заливка памяти
Empty2:
    push offset correctPassword     ; Адрес эталонного пароля

    call enterPassword
    cmp dx, ax
    mov cx, 21ah
    mov cx, 21ah
    jmp Empty

    password db 12, 0, 10 dup(0)    ; Буфер: макс. длина, факт. длина, место для строки

Empty:
    xor bx, bx
    ;cld
    ;je enterPasswordL
    

    call checkPassword

    
checkPassword proc
    pop bx                          ; Извлекаем адрес возврата (временный регистр)
    pop di                          ; Адрес введённой строки (password + 2)
    pop si                          ; Адрес эталонного пароля (correctPassword)

    mov cl, [lenOfcorrectPassword]  ; Длина эталонного пароля
    xor ch, ch                      ; Обнуляем старший байт CX

    cld                             ; Направление вперёд
    repe cmpsb                      ; Сравнение строк
    je equal                        ; Если равны, переходим к equal
    jmp notequal                    ; Если не равны, переходим к notequal

equal:
    mov ah, 09h
    mov dx, offset winner
    int 21h
    jmp exit

notequal:
    mov ah, 09h
    mov dx, offset looser
    int 21h

exit:
    mov ah, 4Ch
    int 21h
checkPassword endp

enterPasswordL:
    push 1f7h
enterPassword proc 

    ; Вывод приглашения
    mov ah, 09h
    mov dx, offset askPasswordMSG
    int 21h

    ; Получение пароля от пользователя
    mov ah, 0Ah
    mov dx, offset password
    int 21h

    ; Передаём адреса через стек
    pop ax
    push offset password + 2        ; Адрес введённой строки
    push ax
    xor dx, dx
    xor ax, ax
    ret

enterPassword endp

end Start
