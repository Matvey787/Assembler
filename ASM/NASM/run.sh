nasm -f elf32 $1.ASM -o obj/$1.o
ld -m elf_i386 -s -o obj/$1 obj/$1.o
./obj/$1
