.model tiny 
.data
    askPasswordMSG db 'Enter the password: ', 0Ah, '$'
    correctPassword db 'DED32'          ; Reference password
    winner db 'You have received admin rights!$'
    looser db 'Are you a real hacker? Looooooser... Ahahha!', 0Ah, 'Incorrect password!$'
    lenOfcorrectPassword db 5           ; Length of the reference password
    hint db 'Hint: behind |000001F5 33DB xor bx,bx|  and  |00000220 50 push ax|  29 instructions$'
.code
org 100h 

Start:
    mov ax, cs
    mov ds, ax                      ; DS = CS
    mov es, ax                      ; ES = CS (for cmpsb)
    jmp Empty2
    trash db 200 dup(0)            ; Memory fill
Empty2:
    push offset correctPassword     ; Address of the reference password

    call enterPassword
    cmp dx, ax
    mov cx, 21ah
    mov cx, 21ah
    jmp Empty

    password db 12, 0, 10 dup(0)    ; Buffer: max length, actual length, space for the string

Empty:
    xor bx, bx
    ;cld
    ;je enterPasswordL
    

    call checkPassword

    
checkPassword proc
    pop bx                          ; Retrieve return address (temporary register)
    pop di                          ; Address of the entered string (password + 2)
    pop si                          ; Address of the reference password (correctPassword)

    mov cl, [lenOfcorrectPassword]  ; Length of the reference password
    xor ch, ch                      ; Clear the high byte of CX

    cld                             ; Direction forward
    repe cmpsb                      ; Compare strings
    je equal                        ; If equal, jump to equal
    jmp notequal                    ; If not equal, jump to notequal

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

    ; Display prompt
    mov ah, 09h
    mov dx, offset askPasswordMSG
    int 21h

    ; Get password from user
    mov ah, 0Ah
    mov dx, offset password
    int 21h

    ; Pass addresses through stack
    pop ax
    push offset password + 2        ; Address of the entered string
    push ax
    xor dx, dx
    xor ax, ax
    ret

enterPassword endp

end Start
