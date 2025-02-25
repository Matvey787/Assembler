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
![This programm](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM)
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