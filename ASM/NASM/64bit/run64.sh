nasm -f elf64 $1.ASM -o $1_64.o
ld $1_64.o -o $1_64
./$1_64
