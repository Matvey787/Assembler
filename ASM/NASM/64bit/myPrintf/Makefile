# Compiler and flags
CXX = gcc
ASMFUNCS = obj/myPrintf.o
SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
CXXFLAGS = -Wall -Wextra -no-pie
          #-D _DEBUG -lncurses -ggdb3 -std=c++17 -O0  -Weffc++                 \
          #-Waggressive-loop-optimizations                                    \
          #-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
          #-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
          #-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
          #-Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked \
          #-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
          #-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
          #-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
          #-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast \
          #-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
          #-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation \
          #-fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
          #-Wlarger-than=81920 -Wstack-usage=81920  -Werror=vla  -pie -fPIE

# Object files
OBJECTS = obj/test.o $(ASMFUNCS)

# Default target
all: init out

# Link
out: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(CXXFLAGS) $(SFML_AND_SDL2) $(SANITIZER)

# Compile source files
obj/%.o: %.c
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(SANITIZER)

obj/%.o: %.ASM
	nasm -f elf64 $< -o $@

# Clean
clean:
	rm -rf out *.o

init:
	mkdir -p obj