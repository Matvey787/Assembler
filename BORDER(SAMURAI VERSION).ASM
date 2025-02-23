.model tiny
.data
    SCREEN_WIDTH equ 160
    START_Y_POS  equ 2
    Buffer db 128 dup(0)                    ; Buffer for command line arguments
    NoArgsMsg db 'No arguments provided.$'  ; if no argumets provided
; ------------------------------------------------------------------------------------
    strData db 0, 0, 0, 0, 0, 0, 0, 0  ; Initialize a byte array in memory
;              |  |  |  |  |  |  |  |
;              |  |  |  |  |  |  |  |_color
;              |  |  |  |  |  |  |  
;              |  |  |  |  |  |  |____symbol
;              |  |  |  |  |  | 
;              |  |  |  |  |  |__________start y coordinate 
;              |  |  |  |  |		
;              |  |  |  |  |_____________start x coordinate
;              |  |  |  |
;              |  |  |  |__________________________________number of cols
;              |  |  |
;              |  |  |_____________________________________number of strs
;              |  |                                                               
;              |  |________________________________________________________cols counter ; // FIXME remove
;              |
;              |___________________________________________________________strs counter   

;-------------------------------------------------------------------------------------

    boxParameters db 0, 0, 0, 0, 0, 0, 0
;                    |  |  |  |  |  |  |
;                    |  |  |  |  |  |  |_____just a zero
;                    |  |  |  |  |  |
;                    |  |  |  |  |  |____type
;                    |  |  |  |  |
;                    |  |  |  |  |____start Y position
;                    |  |  |  |
;                    |  |  |  |_______start X position
;                    |  |  |
;                    |  |  |_____color
;                    |  |
;                    |  |__height
;                    |_____width
    
    boxBorderSymbols db 11 dup(0)  ; Initialize box in memory

    ; ---------- phrase struct ----------
    phrase db 256 dup(0) ; // FIXME no copy
    lngth db 1 dup(0)
    ; -----------------------------------

    ; type 1
    phrase1 db 'Python$'
    boxBorderSymbols1 db 3,3,3,3, ' ', 3,3,3,3,'$'
    ; recommended color 10000100b = 84h

    ; type 2
    phrase2 db 'C++$'
    boxBorderSymbols2 db 176, 176, 176, 176, 177, 176, 176, 176, 176,'$'
    ; recommended color 83h

    ; -----------------------------------

    ; macro for calling PrintBoxChars
CreateBoxPartMacro macro OffsetForOuter, OffsetForInner,
                         fixOuter, fixInner, offsetStartX, offsetStartY, symbolOffset,  colorOffset

    mov al, [boxParameters + OffsetForOuter]    ; set number of iterations for strs
    sub al, fixOuter
    mov ah, [boxParameters + OffsetForInner]    ; set number of iterations for cols
    sub ah, fixInner
    mov byte ptr [strData + 2], al
    mov byte ptr [strData + 3], ah

    mov bl, [si + symbolOffset]                 ; set char for printing
    mov bh, [boxParameters + colorOffset]       ; set color

    mov al, [boxParameters + 3]                 ;<---|
    add al, offsetStartX                        ;<---| calculate start position of box part
    mov ah, [boxParameters + 4]                 ;<---|
    add ah, offsetStartY                        ;<---|

    mov byte ptr [strData + 4], al          ; start x coordinete of printing str
    mov byte ptr [strData + 5], ah          ; start y coordinate of printing str
    mov byte ptr [strData + 6], bl
    mov byte ptr [strData + 7], bh

    call PrintBoxChars

endm

PrintSingleCharMacro macro offsetStartX, offsetStartY, symbolOffset, colorOffset

    mov bl, [si + symbolOffset]                 ; set char for printing
    mov bh, [boxParameters + colorOffset]       ; set color

    add ah, offsetStartY                        ; <---|
    add al, offsetStartX                        ; <---| calculate start position of box part
    mov ah, [boxParameters + 4]                 ; <---|
    mov al, [boxParameters + 3]                 ; <---|

    mov byte ptr [strData + 4], al          ; start x coordinete of printing str ; // FIXME arguments in registers
    mov byte ptr [strData + 5], ah          ; start y coordinate of printing str
    mov byte ptr [strData + 6], bl
    mov byte ptr [strData + 7], bh

    call PrintSingleChar

endm

.code
org 100h
Start:
    call GetCommandArgs
    mov ah, 09h                 ; DOS function for printing string
    lea dx, boxBorderSymbols    ; move offset of Buffer to dx
    int 21h         
    call CenteringOfBox

    ;mov byte ptr [boxParameters+0], 15 ; width
    ;mov byte ptr [boxParameters+1], 9  ; height
    call BoxInfoProc


    ; Program termination
    mov ax, 4C00h
    int 21h

;|--------------------------------------------------|
;| Calculates the start position for centering box  |
;| the box on the screen.                           |
;| Entry:                                           |
;|   - boxParameters: width of the box (1 byte)     |
;|   - SCREEN_WIDTH: screen width (160 bytes)       |
;| Exit:                                            |
;|   - boxParameters + 3: start X position          |
;|   - boxParameters + 4: start Y position          |
;| Destr: AX, BX                                    |
;|--------------------------------------------------|


CenteringOfBox proc
    ; Calculate the X offset
    mov bl, [boxParameters]     ; load box width
    shr bl, 1
    mov al, SCREEN_WIDTH     ; Load the box width
    shr al, 1
    sub al, bl
    shr al, 2

    ; Store the calculated offset for centering box
    mov [boxParameters + 3], al
    mov [boxParameters + 4], START_Y_POS

    ret
CenteringOfBox endp

;|-----------------------------|
;| Get command line arguments  |
;| Entry: None                 |
;| Exit:  None                 |
;| Destr: AX, CX, SI, DI, ES   |
;|-----------------------------|

GetCommandArgs proc
    mov ax, ds ; save data segment to ax
    mov es, ax
    mov cl, es:[80h] ; get number of command line arguments

    cmp cl, 0   ; if no arguments exit 
    je NoArgs   

    mov si, 81h ; save offset (first letter of the string in cmd line)
    lea di, Buffer      ; DI points to a buffer to store arguments

    ; copy arguments to buffer
CopyArguments:
    lodsb               ; Load byte from [SI] into AL, increment SI

    cmp al, 0Dh         ; if we have reached the end of the line (all args has been copied)
    je Done

    stosb               ; Store byte from AL to [DI], increment DI

    loop CopyArguments  ; repeat until we have copied all arguments 
                        ; (decreases CX, if after CX > 0 go to the label)
    
Done:
    ; add zero byte to the of buffer
    mov byte ptr [di], 0

    lea si, Buffer      ; SI point to the start of the buffer
    call ParseNumbers      

    ret                 ; return from GetCommandsArgs
NoArgs:
    ; if no args, print message about it
    mov ah, 09h         ; DOS function for printing string
    lea dx, NoArgsMsg   ; get offset of the message
    int 21h             ; call DOS interrupt
    ret
GetCommandArgs endp

;|-------------------------------|
;| Parse numbers from arguments  |
;| Entry: SI = pointer to buffer |
;| Exit:  None                   |
;| Destr: AX, BX, SI             |
;|-------------------------------|

ParseNumbers proc
    mov bl, 10
    lodsb ; skip first space

    ; read first number width to boxParameters[0]
    readNum1:
    mov al, [boxParameters]
    mul bl
    mov [boxParameters], al
    lodsb                   ; Load byte from [SI] into AL, increment SI
    sub al, '0'             ; convert char to digit
    add [boxParameters], al
    mov al, [si]
    cmp al, ' '
    jne readNum1

    lodsb ; skip space between numbers

    ; read second number height to boxParameters[1]
    readNum2:
    mov al, [boxParameters + 1]   ; <---|
    mul bl                        ; <---| multiply previous digit by 10
    mov [boxParameters + 1], al   ; <---|

    lodsb                   ; Load byte from [SI] into AL, increment SI
    sub al, '0'             ; convert char to digit
    add [boxParameters + 1], al   ; add new digit to the number

    mov al, [si]            ; <---|
    cmp al, ' '             ; <---| compare next char with space
    jne readNum2            ; <---|

    lodsb ; skip space between numbers

    ; get first digit of hex number
    mov bl, 16      ; because now it's FIRST digit of two-hex number

    lodsb           ; load byte from [SI] into AL, increment SI
    cmp al, '9'     ; compare with '9'
    ja isABCDEF1       ; if less or equal, go to isNum
    call isNum
    jmp continue1
isABCDEF1:
    call isChar
continue1:
    ; get second digit of hex number
    xor bl, bl      ; <---| clear bl because now it's SECOND digit of two-hex number
    inc bl          ; <---|

    lodsb           ; load byte from [SI] into AL, increment SI ; // FIXME copypast
    cmp al, '9'     ; compare with '9'
    ja isABCDEF2     ; if less or equal, go to isNum
    call isNum
    jmp continue2
isABCDEF2:
    call isChar
continue2:

    lodsb               ; skip space between numbers

    ; save type of border
    lodsb
    sub al, '0'
    mov [boxParameters + 5], al  

    call FindType

    ret                 ; parsing numbers has been ended
ParseNumbers endp

FindType proc
    cmp al, 0
    jne NotType0

    lodsb               ; skip space between number and string

    call ParseStrings
    ret

NotType0: ; // FIXME copypast
    cmp al, 1
    jne NotType1

    lea di, boxBorderSymbols    ; copy from boxBorderSymbols1 to boxBorderSymbols
    lea si, boxBorderSymbols1
    call CopyString

    lea di, phrase              ; copy from phrase1 to phrase
    lea si, phrase1
    call CopyString

    ret

NotType1:
    cmp al, 2
    jne NotType2

    lea di, boxBorderSymbols    ; copy from boxBorderSymbols1 to boxBorderSymbols
    lea si, boxBorderSymbols2
    call CopyString

    lea di, phrase              ; copy from phrase1 to phrase
    lea si, phrase2
    call CopyString
    ret

NotType2:
    ret
FindType endp

;|------------------------------|
;| Convert character to digit   |
;| Entry: AL = num as character |
;|        BL = multiplier       |
;| Exit:  None                  |
;| Destr: AX, BL                |
;|------------------------------|

isNum proc ; // FIXME macros
    sub al, '0'             ; convert digit char to digit
    mul bl                  ; multiply previous digit by 16 if it's first digit or 0 if it's second
    add [boxParameters + 2], al   ; add new digit to the number
    ret
isNum endp

;|-----------------------------|
;| Convert hex char to digit   |
;| Entry: AL = character (A-F) |
;|        BL = multiplier      |
;| Exit:  None                 |
;| Destr: AX, BL               |
;|-----------------------------|

isChar proc
    sub al, '1'             ; convert char (A, B, C, D, E or F) to digit
    mul bl                  ; multiply previous digit by 16 if it's first digit or 0 if it's second ;; //FIXME all muls 2^n shl
    add [boxParameters + 2], al   ; add new digit to the number
    ret
isChar endp

;|-------------------------------|
;| Parse strings from arguments  |
;| Entry: SI = pointer to buffer |
;| Exit:  None                   |
;| Destr: AX, DI, SI             |
;|-------------------------------|

ParseStrings proc
    cmp [boxParameters + 3], 0           ; <---| if type was not 0 (custom type) so skip all func
    jne ParseStringsEnd                 ; <---|

    ; first string is a border eadges
    lea di, boxBorderSymbols            ; DI point to the start of the boxBorderSymbols
    call CopyString                     ; call func for copying string

    ; second string is a phrase
    lea di, phrase                      ; DI point to the start of the phrase
    call CopyString                     ; call func for copying string

ParseStringsEnd:
    ret                                 ; return from function
ParseStrings endp

;|---------------------------------|
;| Copy string to destination      |
;| Entry: SI = source pointer      |
;|        DI = destination pointer |
;| Exit:  None                     |
;| Destr: AX, DI, SI               |
;|---------------------------------|

CopyString proc
    mov [lngth], 0
CopyLoop:
    lodsb               ; load byte from [SI] into AL, increment SI

    ;cmp al, ' '        ; <---| 
    ;je CopyDone        ; <---| finish copying if in ax space (end of first str) or
    cmp al, 0Dh         ; <---| 0Dh ~ \n (end of second str)
    je CopyDone         ; <---| 
    cmp al, 0
    je CopyDone
    cmp al, '$'
    je CopyDone

    inc [lngth]

    stosb               ; store byte from AL to [DI], increment DI
    jmp CopyLoop        ; continue copying
CopyDone:

    ; add '$' as the end of the string
    mov byte ptr [di], '$'
    ret                 ; return from function
CopyString endp

;|--------------------------------------------|
;| Prints a single character at the specified |
;| position with the specified color.         |
;| Entry:                                     |
;|   - strData:                               |
;|     +4: start X position                   |
;|     +5: start Y position                   |
;|     +6: character to print                 |
;|     +7: color attribute                    |
;| Exit:                                      |
;|   None                                     |
;| Destr: AX, BX, CX                          |
;|--------------------------------------------|

PrintSingleChar proc
    ; Calculate the position in video memory
    mov al, [strData + 5]        ; Load start Y position
    mov ah, 0                    ; Clear AH for multiplication
    mov cx, SCREEN_WIDTH         ; Number of bytes per row (160 bytes)
    mul cx                       ; AX = Y * 160
    mov bx, ax                   ; BX = Y * 160

    mov al, [strData + 4]        ; Load start X position
    mov ah, 0                    ; Clear AH for multiplication
    mov cx, 2                    ; Each character takes 2 bytes
    mul cx                       ; AX = X * 2
    add bx, ax                   ; BX = (Y * 160) + (X * 2)

    ; Load character and color
    mov al, [strData + 6]        ; Load character to print
    mov ah, [strData + 7]        ; Load color attribute

    ; Draw the character
    call DrawCh                  ; Call DrawCh to draw the character

    ret
PrintSingleChar endp

; |----------------------------------------------------------|
; | Draws a box and text inside it                           |
; | Entry:                                                   |
; |   - boxParameters: width and height of the box (2 bytes) |
; |   - box: characters for drawing the box                  |
; |   - phrase: text to display inside the box               |
; | Exit: None                                               |
; | Destr: AX, BX, CX, DX, SI                                |
; |----------------------------------------------------------|

BoxInfoProc proc
    mov dx, offset boxBorderSymbols
    mov si, dx

    ;-------------------- top center line --------------------
    CreateBoxPartMacro 6, 0, 0, 1, 0, 0, 1, 2 ; // FIXME constants

    ;-------------------- central box of symbols --------------------
    CreateBoxPartMacro 1, 0, 2, 1, 1, 1, 4, 2

    ;-------------------- bottom center line --------------------
    mov cl,  [boxParameters + 1]
    dec cl
    CreateBoxPartMacro 6, 0, 0, 0, 0, cl, 7, 2
    
    ;-------------------- right center line --------------------
    mov cl,  [boxParameters]
    dec cl
    CreateBoxPartMacro 1, 6, 1, 0, cl, 1, 5, 2

    ;-------------------- left center line --------------------
    CreateBoxPartMacro 1, 6, 1, 0, 0, 1, 3, 2

    ;-------------------- left top corner --------------------

    ;CreateBoxPartMacro 6, 6, 0, 0, 0, 0, 0, 2
    PrintSingleCharMacro 0, 0, 0, 2
    
    ;-------------------- right top corner --------------------
    mov cl,  [boxParameters]
    dec cl
    ;CreateBoxPartMacro 6, 6, 0, 0, cl, 0, 2, 2
    PrintSingleCharMacro cl, 0, 2, 2
    
    ;-------------------- left bottom corner --------------------
    mov cl,  [boxParameters + 1]
    dec cl
    ;CreateBoxPartMacro 6, 6, 0, 0, 0, cl, 6, 2
    PrintSingleCharMacro 0, cl, 6, 2

    
    ;-------------------- right bottom corner --------------------
    mov cl,  [boxParameters + 1]
    dec cl
    mov ch,  [boxParameters]
    dec ch
    ;CreateBoxPartMacro 6, 6, 0, 0, ch, cl, 8, 2
    PrintSingleCharMacro ch, cl, 8, 2

    ;-------------------- print phrase in box --------------------

    mov al, [boxParameters + 1]
    shr al, 1
    mov bh, al

    xor ax, ax
    mov al, [boxParameters]
    sub al, [lngth]
    shr al, 1

    mov ah, bh
    add al, [boxParameters + 3] ; <---| add box offset
    add ah, [boxParameters + 4] ; <---|

    mov dx, offset phrase
    mov si, dx
phraseFor:
    mov byte ptr [strData + 2], 1    ; Set value for outer loop counter
    mov byte ptr [strData + 3], 1    ; Set value for inner loop counter
    mov byte ptr [strData + 4], al
    mov byte ptr [strData + 5], ah
    mov bl, [si]                   ; char to print
    mov byte ptr [strData+6], bl   ; Load a char byte from memory
    mov byte ptr [strData+7], 0fh  ; Set color to the char

    push ax
    push bx
    call PrintBoxChars 	           ; Call the PrtStr procedure
    pop bx                         ; Restore the value of BL 
    pop ax                         ; Restore the value of AX 
    
    inc al                         ; Increment the X position 
    inc si                         ; Increment the pointer to the next character 
    mov bl, [si]                   ; Load the next character for scaning the end of phrase 
    cmp bl, '$'
    jne phraseFor

    ret
BoxInfoProc endp
    
;|-----------------------------|
;| Draw a char in video memory |
;| Entry: AL = char to write   |
;|        AH = color attribute |
;|        BX = position        |
;| Exit:  None                 |
;| Destr: None                 |
;|-----------------------------|
DrawCh proc ; // FIXME macros
    push cx
    mov cx, 0B800h ; // FIXME const
    mov es, cx
    mov es:[bx], ax
    pop cx
    ret
DrawCh endp

;|-----------------------------|
;| Print a string of characters|
;| Entry: strData              |
;| Exit:  None                 |
;| Destr: AX, BX, CX, DX       |
;|-----------------------------|
PrintBoxChars proc
    mov byte ptr [strData], 0    ; Outer loop counter
    mov byte ptr [strData+1], 0  ; Inner loop counter

outForY:
    ; Calculate Y position
    mov al, [strData]            ; Load outer loop counter
    add al, [strData + 5]
    mov ah, 0                    ; Clear AH for multiplication
    mov cx, SCREEN_WIDTH         ; Number of bytes per row
    mul cx                       ; AX = Y * 160
    mov bx, ax
    mov al, [strData + 4]
    mov ah, 0
    mov cx, 2
    mul cx
    add bx, ax                   ; BX = starting position in video memory

outForX:
    ; Draw character
    mov al, [strData+6]               ; Character 'A' with attribute 0DAh
    mov ah, [strData+7]
    call DrawCh                  ; Call DrawCh to draw character
    add bx, 2                    ; Move to the next position

    ; Increment inner loop counter
    mov al, [strData+1]
    add al, 1
    mov [strData+1], al

    ; Check condition to exit inner loop
    mov ah, [strData+3]          ; Load value for comparison
    cmp al, ah
    jb outForX                   ; If AL < [strData+3], continue inner loop

    ; Reset inner loop counter
    mov byte ptr [strData+1], 0

    ; Increment outer loop counter
    mov al, [strData]
    add al, 1
    mov [strData], al

    ; Check condition to exit outer loop
    mov ah, [strData+2]          ; Load value for comparison
    cmp al, ah
    jb outForY                   ; If AL < [strData+2], continue outer loop

    ret
PrintBoxChars endp

end Start
