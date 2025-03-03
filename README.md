# Table of Contents
- [DOSBox](#dosbox)
- [TASM](#tasm)
  - [DRAWING BORDER](#drawing-border)
    - [Versions](#versions)
    - [Run program](#run-program)
- [CRACKalca](#crackalca)
  - [Description](#description)
  - [Compilation](#compilation)
  - [My own programm for cracking](#my-own-programm-for-cracking)
- [NASM](#nasm)
  - [Repeat](#repeat)
  - [Shift loop](#shift-loop)

# DOSBox
![Download DOSBox (windows)](https://github.com/Matvey787/Assembler/blob/main/DOSBox.exe)

# TASM
## DRAWING BORDER
### Versions
Program for printing border with a text in the center in it in DOS video mem. 
- <b>BORDER (SAMURAI VERSION)</b> <br>
![This programm](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(SAMURAI%20VERSION).ASM)
 in most uses operative memory acess. This makes it slower, larger, but at least you 
don't have to constantly monitor the register values.
- <b>BORDER (FAST VERSION)</b> <br>
[This programm](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM)
only uses registers. This makes it faster, smaller, but you have to constantly monitor the register values.
### Run program
1. Run DOSBox
2. Create *.com file (ASM -> OBJ -> COM)
3. Run by this command for example:
```
yourCOMFileName.com 16 7 04 0 1234_6789 Hello,world!
```
- First parametr - width <br>
- Second parametr - height <br>
- Third parametr - color of border <br>
    **Example of color: 1|101|1|010** <br>
    This is an 8-bit color attribute value: <br>
    - `1` (bit 7): Text blinking is enabled.  
    - `101` (bits 4–6): Magenta background (5 in decimal).  
    - `1` (bit 3): Bright (intense) text color.  
    - `010` (bits 0–2): Green text (2 in decimal).  
    In decimal, this is `218` or `DA` in hex. For example, instead of `04`, you can use `DA` to get blinking bright green text on a magenta background.
- Fourth parametr - type of border (1, 2 or 0 - user own) <br>
>[!NOTE]
> Next parameters needed if at forth 0
- Fifth parametr - border edges</br>
- Sixth parametr - text in border<br>

4. You will see it:
![DOSBox console](https://github.com/Matvey787/Assembler/blob/main/imgs/image.png)

> [!IMPORTANT]
> [Second version](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM) more
stable and can catch incorrect input.

# CRACKalca
## Description
This program is the easiest example of "cracking" programm. It is going throw the code and
change it to work even if user didn't guess the password. The programm simply replaces the
transition adresses of commands such as 'JNE' (or 'JNZ') in the COM file. <br> The example of 
[source file](https://github.com/Matvey787/Assembler/blob/main/TRYTOHACK/PROGFORHACK.COM) which
needs to be hacked. <br>
First of all I need to understand how the com file works. So I do disassembly of it:
>[!NOTE]
> Download nasm to Linux first of all.
```
ndisasm -b 16 -o 100h com/file/path.COM > asm/file/path.asm
```
- 100h mean offset of the code in the com file.
- -b 16 mean 16-bit code (optionally it could be 32 or 64-bit code)

After it you can find [*.asm file](https://github.com/Matvey787/Assembler/blob/main/TRYTOHACK/PROGFORHACK.asm) with disassembled code. Now you can probe it and find it's vulnerabilities. After the first 
viewing I found interesting strings: <br>

![asmStrings](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs.png)

This is what we need, this is where the comparison of two lines occures, the addresses 
of which are tempurary stored in `si` and `di` registers. And if at least one symbol
of them not match the programm will go to label which had written after `jnz` (or `jne`).
We see a successful transition right after `cmp al, 0x0`, because if all symbols were the same
we go to the and it's 00h. So we need to change `jnz 0x173` -> `jnz 0x165`.

>[!WARNING]
> This program is not intended for use in any other way except for educational purposes.
> Everything that is here is purely for entertainment purposes.

## Compilation
To compile this project you need to install SFML-library.
```
    sudo apt-get install libsfml-dev
```

And you need to install SDL2 and SDL2_mixer libraries:
```
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-mixer-dev
```

After installing SFML-library, SDL2, SDL2_mixer you can compile this project with and run it:
```
    g++ CRACKalka.cpp -o out -lsfml-graphics -lsfml-window -lsfml-system -lSDL2 -lSDL2_mixer
    ./out path/to/com/file.com
```
Or you can use `make` to build this project:
```
    make
    ./out path/to/com/file.com
```
## My own programm for cracking
I also create interesting programm for cracking. It's [here](https://github.com/Matvey787/Assembler/blob/main/TRYTOHACK/MYPROGFORHACKING.COM). It's really hard to find vulnerabilities in it.
### First vulnerability
After dissassembly of it you can see it:

![asmStrings2](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs2.png)
![asmStrings2](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs3.png)

If you look at the code you can find some oddities. 2 segments of data are located directly in the code. The first such segment is just trash, I added it to confuse people who disassembles it. After
it you can see you can see the calling of function `call 0x22a`. If go to this address you can see
 it:

![asmStrings2](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs4.png)

This part of code print something `mov ah, 0x9` (DOS func for print) and get something `mov ah,0xa`.
So more likely this function is for input password from user. <b>It is clear that the password address is pushed into the stack.</b><br>

If you look closely, you can find a function at the address `0x1fa` <b>that compares `di` and `si`, and they get there from the stack, into which both the reference password and the entered password are pushed.</b>

Before `0x22a` you can see `mov word [bp+0x2],0x1f1`. 

![asmStrings5](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs5.png)

This pushes the address of the command to call the comparison function. <br>
So the answer is to enter firstly enter `1234567890t)`.
- We erase useless `xor bx, bx`

![asmStrings6](https://github.com/Matvey787/Assembler/blob/main/imgs/asmStrs6.png)

- `t)` is `74 29` for *.com file. `74` - `je`, `29` - offset to `0x22a`. You can find it as a hint
in data-segment:

![comHint](https://github.com/Matvey787/Assembler/blob/main/imgs/cormHint.png)

After this jump we got to function `0x22a`, then ret goes to `1f7`. So it calls `0x1fa`, comparison our and our password.
### Second vulnerability
Thanks to expert, white hacker [Egor](https://github.com/ZEVS1206), he founded another vulnerability in this programm. <br>
You can easily enter this password <b>1234567890[backtick]</b>. Because of 11 symbols > 10 symbol, so <b>[backtick]</b> writes to *.com
file 60h. So it changes `xor bx, bx` to `pusha`. So programm pushes all registers, they are zero, and then
function `1f7` pop two registers and compares them instead of comparing the reference password and the password entered by the user.
It also works in every case. Cool!!!

# NASM
## Repeat
[Programm](https://github.com/Matvey787/Assembler/blob/main/ASM/NASM/repeat.ASM) easily repeats the symbol and text which you passed.
## Shift loop
[Programm](https://github.com/Matvey787/Assembler/blob/main/ASM/NASM/shiftLoop.ASM) performs a cyclic shift of 1 in an array of three digits `123` -> `231`.
You can run programs in NASM by such commands:
- Compilation 32-bit programm
```
nasm -f elf32 yourProgramm.ASM -o yourProgramm.o
ld -m elf_i386 -s -o yourProgramm yourProgramm.o
```
- Compilation 64-bit programm
```
nasm -f elf64 yourProgramm.ASM -o yourProgramm.o
ld -m -s -o yourProgramm yourProgramm.o
```