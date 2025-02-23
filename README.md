# DOSBox
![Download DOSBox (windows)](https://github.com/Matvey787/Assembler/blob/main/DOSBox.exe)

# TASM
## DRAWING BORDER
### Versions
Program for printing border with a text in the center in it in DOS video mem. 
- BORDER (SAMURAI VERSION) <br>
![This programm](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(SAMURAI%20VERSION).ASM)
 in most uses operative memory acess. This makes it slower, larger, but at least you 
don't have to constantly monitor the register values.
- BORDER (FAST VERSION) <br>
![This programm](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM)
only uses registers. This makes it faster, smaller, but you have to constantly monitor the register values.
### Run program
- Run DOSBox
- Create *.com file (ASM -> OBJ -> COM)
- Run by this command for example:
```
yourCOMFileName.com 16 7 04 0 1234_6789 Hello,world!
```
You will see it:
![DOSBox console](https://github.com/Matvey787/Assembler/blob/main/imgs/image.png)

> [!IMPORTANT]
> [Second version](https://github.com/Matvey787/Assembler/blob/main/ASM/TASM/BORDER(COOL).ASM) more
stable and can catch incorrect input.
## 