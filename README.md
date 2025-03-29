# Table of Contents

- [DOSBox](#dosbox)
- [TASM](#tasm)
  - [DRAWING BORDER](#drawing-border)
    - [Versions](#versions)
    - [Run program](#run-program)
- [CRACKalca](#crackalca)
  - [Description](#description)
  - [Compilation](#compilation)
  - [My own program for cracking](#my-own-program-for-cracking)
- [NASM](#nasm)
  - [Repeat](#repeat)
  - [Shift loop](#shift-loop)

## DOSBox

![Download DOSBox (windows)](https://github.com/Matvey787/Assembler/blob/main/DOSBox.exe)

## TASM

## DRAWING BORDER

### Versions

Program for printing a border with text in the center in DOS video memory.

- **BORDER (SAMURAI VERSION)**  
  [Program](ASM/TASM/BORDER(SAMURAI%20VERSION).ASM)
  mostly uses operative memory access. This makes it slower and larger, but at least you
   don't have to constantly monitor the register values.
  
- **BORDER (FAST VERSION)**  
[Program](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM)
only uses registers. This makes it faster and smaller, but you have to constantly monitor the register values.

### Run program

1. Run DOSBox
2. Create a *.com file (ASM -> OBJ -> COM)
3. Run with the command, for example:

```bash
yourCOMFileName.com 16 7 04 0 1234_6789 Hello, world!
```

- First parameter - width  
- Second parameter - height  
- Third parameter - color of the border  
    **Example of color: 1|101|1|010**  
    This is an 8-bit color attribute value:  
  - `1` (bit 7): Text blinking is enabled.  
  - `101` (bits 4–6): Magenta background (5 in decimal).  
  - `1` (bit 3): Bright (intense) text color.  
  - `010` (bits 0–2): Green text (2 in decimal).

    In decimal, this is `218` or `DA` in hex. For example, instead of `04`, you can use `DA` to get blinking bright green text on a magenta background.

- Fourth parameter - type of border (1, 2, or 0 - user-defined)

>[!NOTE]
> The next parameters are needed if the fourth parameter is 0

- Fifth parameter - border edges  
- Sixth parameter - text in the border  

4. You will see:

![DOSBox console](https://github.com/Matvey787/Assembler/blob/main/imgs/image.png)

> [!IMPORTANT]
> The [second version](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM) is more
stable and can handle incorrect input.

## CRACKalca

### Description

Program is the easiest example of a "cracking" program. It goes through the code and
modifies it to work even if the user didn't guess the password. The program simply replaces the
jump addresses of commands such as 'JNE' (or 'JNZ') in the COM file.  
Here is an example of the [source file](https://github.com/Matvey787/Assembler/blob/main/TRYTOHACK/PROGFORHACK.COM) that
needs to be hacked.  
First of all, I need to understand how the COM file works. So I disassemble it:
>[!NOTE]
> Download NASM to Linux first.

``` bash
ndisasm -b 16 -o 100h com/file/path.COM > asm/file/path.asm
```

- 100h means the offset of the code in the COM file.
- -b 16 means 16-bit code (optionally it could be 32 or 64-bit code)

After this, you can find the [*.asm file](https://github.com/Matvey787/Assembler/blob/main/TRYTOHACK/PROGFORHACK.asm) with the disassembled code. Now you can analyze it and find its vulnerabilities. After the first
 viewing, I found interesting strings:  

![asmStrings](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs.png)

This is what we need; this is where the comparison of two lines occurs, the addresses
 of which are temporarily stored in the `si` and `di` registers. If at least one symbol
does not match, the program will jump to the label written after `jnz` (or `jne`).
We see a successful transition right after `cmp al, 0x0`, because if all symbols were the same,
we go to the end, which is 00h. So we need to change `jnz 0x173` -> `jnz 0x165`.

>[!WARNING]
> Program is not intended for use in any way except for educational purposes.
> Everything here is purely for entertainment purposes.

### Compilation

To compile the project, you need to install the SFML library.

```bash
    sudo apt-get install libsfml-dev
```

And you need to install the SDL2 and SDL2_mixer libraries:

```bash
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-mixer-dev
```

After installing the SFML library, SDL2, and SDL2_mixer, you can compile the project and run it:

```bash
    g++ CRACKalka.cpp -o out -lsfml-graphics -lsfml-window -lsfml-system -lSDL2 -lSDL2_mixer
    ./out path/to/com/file.com
```

Or you can use `make` to build the project:

```bash
    make
    ./out path/to/com/file.com
```

### My own program for cracking

I also created an interesting program for cracking. It's [here](https://github.com/Matvey787/Assembler/blob/main/TRYTOHACK/MYPROGFORHACKING.COM). It's really hard to find vulnerabilities in it.

#### First vulnerability

After disassembly, you can see:

![asmStrings2](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs2.png)
![asmStrings2](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs3.png)

If you look at the code, you can find some oddities. Two segments of data are located directly in the code. The first such segment is just trash; I added it to confuse people who disassemble it. After
it, you can see the calling of the function `call 0x22a`. If you go to this address, you can see:

![asmStrings2](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs4.png)

This part of the code prints something `mov ah, 0x9` (DOS function for print) and gets something `mov ah,0xa`.
So more likely this function is for inputting a password from the user. **It is clear that the password address is pushed onto the stack.**

If you look closely, you can find a function at the address `0x1fa` **that compares `di` and `si`, and they get there from the stack, into which both the reference password and the entered password are pushed.**

Before `0x22a`, you can see `mov word [bp+0x2],0x1f1`.

![asmStrings5](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs5.png)

This pushes the address of the command to call the comparison function.  
So the answer is to enter `1234567890t)`.

- We erase the useless `xor bx, bx`

![asmStrings6](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs6.png)

- `t)` is `74 29` for a *.com file. `74h` - `je`, `29h` - offset to `0x22a`. You can find it as a hint
in the data segment:

![comHint](https://github.com/Matvey787/Assembler/blob/main/imgs/cormHint.png)

After the jump, we go to function `0x22a`, then `ret` goes to `1f7`. So it calls `0x1fa`, comparing our password and the reference password.

#### Second vulnerability

Thanks to expert white hacker [Egor](https://github.com/ZEVS1206), he found another vulnerability in the program.  
You can easily enter the password **1234567890[backtick]**. Because 11 symbols > 10 symbols, so **[backtick]** writes to the *.com
file as 60h. So it changes `xor bx, bx` to `pusha`. So the program pushes all registers, they are zero, and then
function `1f7` pops two registers and compares them instead of comparing the reference password and the password entered by the user.
It also works in every case. Cool!!!

## NASM

### 64-bit

#### My printf function

[Printf](ASM/NASM/64bit/myPrintf/myPrintf.ASM) is one of the popular C functions rewritten by me.
You can see specificators in the table below:

| Specificator | Input                  | Output               |
|--------------|------------------------|----------------------|
| %d           | Signed decimal number  | Signed decimal number|
| %b           | Unsigned decimal number| Binary number        |
| %o           | Unsigned decimal number| Octal number         |
| %x           | Unsigned decimal number| Hexadecimal number   |
| %c           | char (one byte)        | char                 |
| %s           | char array             | char array           |

>[!NOTE]
> `%%` also supported  

#### Mandelbrot

  ![Mandelbrot](ASM/NASM/64bit/Buterbrod/imgs/Mandelbrot.png)
  To see the description of the program, see [repo](ASM/NASM/64bit/Buterbrod/).
  
### 32-bit

#### Repeat

[Program](ASM/NASM/32bit/repeat.ASM) easily repeats the symbol and text which you passed.

#### Shift loop

[Program](ASM/NASM/32bit/shiftLoop.ASM) performs a cyclic shift of 1 in an array of three digits `123` -> `231`.
You can run programs in NASM with these commands:

- Compilation of a 32-bit program

```bash
nasm -f elf32 yourProgram.ASM -o yourProgram.o
ld -m elf_i386 -s -o yourProgram yourProgram.o
```

- Compilation of a 64-bit program

```bash
nasm -f elf64 yourProgram.ASM -o yourProgram.o
ld -m -s -o yourProgram yourProgram.o
```
