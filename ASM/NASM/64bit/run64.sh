nasm -f elf64 $1.ASM -o ../obj/$1_64.o
ld ../obj/$1_64.o -o ../obj/$1_64
cd ..
./obj/$1_64
cd - > /dev/null
