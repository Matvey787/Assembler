.model tiny  ; Use the tiny memory model (code and data in one segment)
.data

; Variables to store the original interrupt handler address
old09Ofs dw 0                   ; Offset of the original interrupt 09h handler
old09Seg dw 0                   ; Segment of the original interrupt 09h handler

.code        ; Start of the code segment

org 100h     ; Set the origin (start address) to 100h (for COM files)

Start:
    ; Print a string to the screen
    mov ah, 09h                 ; DOS function 09h: Print string
    mov dx, offset String       ; Load the address of the string into DX
    int 21h                     ; Call DOS interrupt 21h to print the string

    ; Save the original interrupt handler for interrupt 09h (keyboard)
    xor ax, ax                  ; Clear AX (set AX to 0)
    mov es, ax                  ; Set ES to 0 (segment of the interrupt vector table)
    mov bx, 09h * 4             ; Calculate the offset for interrupt 09h in the vector table (09h * 4)

    ; Save the original offset and segment of the interrupt handler
    mov ax, es:[bx]             ; Load the offset of the original handler into AX
    mov old09Ofs, ax            ; Save the offset to the variable old09Ofs
    mov ax, es:[bx + 2]         ; Load the segment of the original handler into AX
    mov old09Seg, ax            ; Save the segment to the variable old09Seg

    ; Install the new interrupt handler for interrupt 09h
    cli                         ; Disable interrupts to prevent race conditions
    mov es:[bx], offset New09   ; Set the offset of the new handler in the vector table
    push cs                     ; Push the code segment (CS) onto the stack
    pop ax                      ; Pop the code segment into AX
    mov es:[bx + 2], ax         ; Set the segment of the new handler in the vector table
    sti                         ; Re-enable interrupts

    ; Make the program resident in memory
    mov ax, 3100h               ; DOS function 31h: Terminate and stay resident
    mov dx, offset EndOfProgram ; Load the end address of the program into DX
    shr dx, 4                   ; Convert the address to paragraphs (divide by 16)
    inc dx                      ; Round up to the next paragraph
    int 21h                     ; Call DOS interrupt 21h to make the program resident

; New interrupt handler for interrupt 09h (keyboard)
New09 proc
    ; Save registers
    push ax bx es

    ; Set the video segment (0B800h for text mode)
    mov ax, 0B800h              ; Load the video segment into AX
    mov es, ax                  ; Move AX into ES (extra segment)

    ; Read the scan code of the pressed key
    in al, 60h                  ; Read the scan code from port 60h (keyboard data port)

    ; Display the scan code in video memory
    mov ah, 0Eh                 ; Set the attribute (color) for the character
    mov bx, 5 * 80 * 2 + 40 * 2 ; Calculate the position on the screen (row 5, column 40)
    mov es:[bx], ax             ; Write the scan code and attribute to video memory

    ; Acknowledge the interrupt to the keyboard controller
    in al, 61h                  ; Read from port 61h (keyboard control port)
    or al, 80h                  ; Set the "acknowledge" bit (bit 7);    80h <=> 10000000
    out 61h, al                 ; Send the acknowledgment back to the controller
    and al, 7Fh                 ; Clear the "acknowledge" bit ;         7Fh  <=> 01111111
    out 61h, al                 ; Send the updated value back to the controller

    mov al, 20h                 ; Load the EOI command (20h) into AL
    out 20h, al                 ; Send the EOI command to port 20h (interrupt controller)

    ; Restore registers
    pop es bx ax
    iret                        ; Return from the interrupt
New09 endp

; Data section
String db 'Program has been started!$' ; Message to display when the program starts

EndOfProgram:                   ; Label marking the end of the program
end Start                       ; End of the program, start execution at the label "Start"