.model tiny
.data

START_Y_POS  equ 160    ; third line
SCREEN_WIDTH equ 80
errDecimal  db 'Error: Invalid decimal number in command line (w_or_h_err)', 0Dh, '$'
errHex      db 'Error: Invalid hex number in command line (collor_err)',     0Dh, '$'
errTypeZero db 'Error: Type is 0 and no edges provided',                     0Dh, '$'
errNoArgs   db 'Error: No arguments provided (Example: *.com 15 7 A9 1)',    0Dh, '$'

    ; type 1
    phrase1 db 'Python'                             ; phrase of type 1
    boxBorderSymbols1 db 3,3,3,3, 177, 3,3,3,3      ; border edges of type 1
    len1 db 6                                       ; len of phrase1
    ; recommended color 10000100b = 84h

    ; type 2
    phrase2 db 'C++'                                                    ; phrase of type 2
    boxBorderSymbols2 db 176, 176, 176, 176, 177, 176, 176, 176, 176    ; border edges of type 2
    len2 db 3                                                           ; len of phrase2
    ; recommended color 83h


;|--------------------------------------|
;| Macro to draw a char in video memory |
;| Entry: DI = address of char to write |
;|        AH = color attribute          |
;|        BX = position                 |
;| Exit:  char on the screen            |
;| Destr: BX                            |
;|--------------------------------------|
DRAWCH macro
    push cx                      ; Save CX
    push ax                      ; Save AX
    mov cx, 0B800h               ; Video memory segment
    mov es, cx                   ; Set ES
    shl bx, 1                    ; Multiply position by 2
    mov al, [di]                 ; Load character from DI into AL
    mov es:[bx], ax              ; Write character and color to video memory
    shr bx, 1                    ; Restore position
    pop ax                       ; Restore AX
    pop cx                       ; Restore CX
endm

SET_TYPE macro index
    lea di, boxBorderSymbols&index  ; Load address of border symbols
    lea si, phrase&index            ; Load address of phrase
    mov cl, len&index               ; Load length of phrase
    xor ch, ch                      ; Clear CH (type info not needed)
    ret                             ; Return from procedure
endm

.code
org 100h

Start:
    call getCmdArgs             ; call function for getting command line args
    push cx                     ; save data of CX register
    xor cx, cx                  ; CX = 0

    ; strings loop
    mov cl, dh                  ; amount of string iterations
    xor bx, bx                  ; BX = 0
    call Centering              ; BX = left top corner
vertical_loop:
    call PrintLine              ; call function printing string
    add bx, SCREEN_WIDTH        ; go to the next line in video memory
    cmp cl, dh                  ; check if it is the first iteration (CX == DH)
    je first_iteration          ; go to label first_iteration
    cmp cl, 2                   ; check if it is pre-last iteration (CX == 2)
    je pre_last_iteration       ; go to label pre_last_iteration

    jmp skip_increment          ; if it is not the first or pre-last iteration

first_iteration:
    add di, 3                   ; increase di by 3 for changing chars line
    jmp skip_increment          ; nothing more to do

pre_last_iteration:
    add di, 3                   ; increase di by 3 for changing chars line

skip_increment:
    loop vertical_loop          ; continue loop if CX - 1 is not zero

    pop cx                      ; restore CX
    call wrPhrase               ; call function printing phrase in border box
    mov ax, 4C00h               ; completion of the program
    int 21h
;|--------------------------------------------------|
;| Count start x and y position of left top corner. |
;| Entry: None                                      |
;| Exit:  phrase on the screen                      |
;| Destr: None                                      |
;|--------------------------------------------------|

Centering proc
    push cx                     ; save cx data     
    mov cx, SCREEN_WIDTH        ; CX = SCREEN_WIDTH
    sub cl, dl                  ; CX = SCREEN_WIDTH - BORDER_WIDTH
    shr cx, 1                   ; CX = (SCREEN_WIDTH - BORDER_WIDTH) / 2
    add cx, START_Y_POS         ; CX = (SCREEN_WIDTH - BORDER_WIDTH) / 2 + Y offset
    add bx, cx                  ; move CX to BX
    pop cx
    ret
Centering endp
;|-------------------------------------------|
;| Write a phrase in the center of the frame |
;| Entry: DI = address of string             |
;|        AH = color attribute               |
;|        BX = position                      |
;|        DL = width of the frame            |
;|        CL = length of the phrase          |
;|        DH = height of the frame           |
;| Exit:  phrase on the screen               |
;| Destr: AX, BX, CX                         |
;|-------------------------------------------|
wrPhrase proc
    push ax
    mov di, si                  ; Now address of phrase in DI
    xor bx, bx                  ; BX = 0
    xor ax, ax                  ; AX = 0

    ; Vertical offset
    mov bl, dh                  ; copy DH (height of border) to BX
    shr bx, 1                   ; get half of height
    mov ax, 80                  ; AX = 80
    push dx
    mul bx                      ; After multiplying in AX smaller part, in DX bigger part
    mov bx, ax                  ; BX = AX
    add bx, dx                  ; BX = AX + DX
    pop dx                      ; restore DX

    ; Horizontal offset
    xor ax, ax
    mov al, dl                   ; copy DL (width of border) to AX
    sub ax, cx                   ; AX = DL - CL (border width - len of phrase)
    shr ax, 1                    ; AX = (DL - CL) / 2
    add bx, ax                   ; BX = (DH / 2) * 80 + (DL - CL) / 2
    pop ax
    call Centering               ; add offset related to the offset of border
wrPhrase_loop:
    DRAWCH                       ; Print word to the screen (было call DrawCh)
    inc di                       ; go to next symbol
    inc bx                       ; add offset for position in video memory
    loop wrPhrase_loop           ; In CX saved len of phrase (while > 0 -> continue)

    ret
wrPhrase endp

; |-----------------------------------------------------|
; | Get arguments from command line                     |
; | Entry:  None                                        |
; | Exit: DL - first  number, width of border           |
; |       DH - second number, height of border          |
; |       AH - third  number, color (hex number)        |
; |       CH - fourth number, type                      |
; |       CL - length of second string                  |
; |       SI - offset to the first letter of second str |
; |       DI - offset to the first letter of first str  |
; | Destr: BX                                           |
; |-----------------------------------------------------|
getCmdArgs proc
    mov bl, ds:[80h]            ; Load length of command line args
    cmp bl, 1                   ; Check if length <= 1 (only space or nothing)
    jbe no_args_error           ; If yes, jump to error
    mov si, 82h                 ; Set a pointer to the start of the command line args
    call parseDecimal           ; Read first decimal number (width)
    mov dl, bl                  ; DL = BL
    call parseDecimal           ; Read second decimal number (height)
    mov dh, bl                  ; DH = BL
    call parseHex               ; Read third hex number (color)
    mov ah, bl                  ; AH = BL
    call parseDecimal           ; Read fourth decimal number (type of border)
    mov ch, bl                  ; CH = BL
    cmp ch, 1                   ; check if it is type 1
    jne NotType1
    SET_TYPE 1                  ; Set type 1 parameters and return
NotType1:
    cmp ch, 2                   ; check if it is type 2
    jne NotType2
    SET_TYPE 2                  ; Set type 2 parameters and return
NotType2:
    cmp ch, 0                   ; check if type is 0
    jne continue_processing     ; if not 0, process custom string
    cmp al, 0Dh                 ; check if it’s newline (\n)
    je type_zero_error          ; if yes, error (no phrase provided)
    dec si                      ; move SI back if not newline
continue_processing:
    xor al, al
    mov di, si                  ; now DI points to the start of first str in cmd line
    push ax
find_end1:
    lodsb                       ; Searching the end of first str
    cmp al, ' '                 ; stop = space
    jne find_end1
    mov ax, si                  ; save start of phrase
    mov bx, ds:[80h]            ; save len of cmd args
    xor bh, bh                  ; BH = 0
    sub ax, 80h                 ; start of phrase - start address of cmd args
    sub bx, ax                  ; len of cmd args - len of passed args = len of phrase
    mov cl, bl                  ; CL = BL
    inc cl                      ; now CL is storing the len of phrase
    pop ax                      ; restore AX
    ret
type_zero_error:
    mov ah, 09h                 ; DOS function to print string
    mov dx, offset errTypeZero  ; Load address of error message
    int 21h                     ; Print error message
    mov ax, 4C01h               ; Exit with error code 1
    int 21h
no_args_error:
    mov ah, 09h                 ; DOS function to print string
    mov dx, offset errNoArgs    ; Load address of error message
    int 21h                     ; Print error message
    mov ax, 4C01h               ; Exit with error code 1
    int 21h
getCmdArgs endp

; |-----------------------------|
; | Parse decimal number        |
; | Entry:  SI - ptr to str     |
; | Exit: BL - result (dec num) |
; | Destr: None                 |
; |-----------------------------|

parseDecimal proc
    xor bx, bx                   ; BX = 0
parse_decimal_loop:
    lodsb                        ; Load character from string into AL
    cmp al, ' '                  ; Check for space
    je parse_decimal_done
    cmp al, 0Dh                  ; Check for \n
    je parse_decimal_done
    cmp al, '0'                  ; Check if character is a digit
    jb parse_decimal_error
    cmp al, '9'
    ja parse_decimal_error
    sub al, '0'                  ; Convert character to digit
    cmp bx, 0                    ; If it's not the first digit, multiply by 10
    je skip_mul
    push ax
    mov ax, bx
    mov cx, 10
    push dx
    mul cx
    pop dx
    mov bx, ax
    pop ax
skip_mul:
    add bx, ax                   ; Add digit to result
    jmp parse_decimal_loop
parse_decimal_done:
    ret
parse_decimal_error:
    mov ah, 09h                  ; DOS function to print string
    mov dx, offset errDecimal    ; Load address of error message
    int 21h                      ; Print error message
    mov ax, 4C01h                ; Exit with error code 1
    int 21h
    ret
parseDecimal endp

; |-------------------------------|
; | Parse hex number              |
; | Entry:  SI - ptr to str       |
; | Exit:   BL - result (hex num) |
; | Destr:  AX                    |
; |-------------------------------|
parseHex proc
    xor bx, bx                   ; Clear BX
    lodsb                        ; Read first digit
    call HexCharToValue          ; Convert character to value
    cmp al, 0FFh                 ; Check if conversion failed
    je parse_hex_error
    add bl, al                   ; Add value of first digit
    shl bl, 4                    ; Shift result left by 4 bits
    lodsb                        ; Read second digit
    call HexCharToValue          ; Convert character to value
    cmp al, 0FFh                 ; Check if conversion failed
    je parse_hex_error
    add bl, al                   ; Add value of second digit
    inc si                       ; Skip space or next char
    ret
parse_hex_error:
    mov ah, 09h                  ; DOS function to print string
    mov dx, offset errHex        ; Load address of error message
    int 21h                      ; Print error message
    mov ax, 4C01h                ; Exit with error code 1
    int 21h
parseHex endp

; |------------------------------------------|
; | Convert char-digit of hex num to a digit |
; | Entry:  AL - symbol                      |
; | Exit:   AL - digit (0xFF if invalid)     |
; | Destr:  None                             |
; |------------------------------------------|
HexCharToValue:
    cmp al, '0'                  ; Check if less than '0'
    jb .invalid
    cmp al, '9'                  ; Check if digit
    jbe .digit
    cmp al, 'A'                  ; Check if less than 'A'
    jb .invalid
    cmp al, 'F'                  ; Check if upper case hex letter
    jbe .upper_letter
    cmp al, 'a'                  ; Check if less than 'a'
    jb .invalid
    cmp al, 'f'                  ; Check if lower case hex letter
    jbe .lower_letter
.invalid:
    mov al, 0FFh                 ; Return 0xFF for invalid character
    ret
.digit:
    sub al, '0'                  ; Convert digit to value
    ret
.upper_letter:
    sub al, 'A' - 10             ; Convert upper case letter to value
    ret
.lower_letter:
    sub al, 'a' - 10             ; Convert lower case letter to value
    ret

;|--------------------------------------|
;| Draw a line of chars in video memory |
;| Entry: DI = address of string        |
;|        AH = color attribute          |
;|        BX = position                 |
;| Exit:  char on the screen            |
;| Destr: None                          |
;|--------------------------------------|

PrintLine proc
    push di cx                   ; Save DI
    xor cx, cx                   ; Clear CX
    DRAWCH                       ; Draw first character (было call DrawCh)
    inc bx                       ; Increment offset
    mov cl, dl                   ; Set line length
    inc di                       ; Move to next character
    sub cl, 2                    ; Decrease length by 2
print_line_loop:
    DRAWCH                       ; Draw character (было call DrawCh)
    inc bx                       ; Increment offset
    loop print_line_loop         ; Repeat loop
    inc di                       ; Move to last character
    DRAWCH                       ; Draw last character (было call DrawCh)
    inc bl                       ; Increment offset
    sub bl, dl                   ; Restore offset
    pop cx di                    ; Restore CX
    ret
PrintLine endp

end Start
