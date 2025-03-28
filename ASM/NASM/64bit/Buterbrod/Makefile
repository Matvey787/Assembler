# Compiler and flags
CXX = clang++
SANITIZER = 
SFML_AND_SDL2 = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lSDL2 -lSDL2_mixer
CXXFLAGS = -I inc -mavx -D _DEBUG -std=c++17 -O3 -Wall -Wextra -Weffc++                 \

# -Wfloat-equal 
# Object files
OBJECTS = obj/buterbrod_spd2trueIntr.o

# Default target
all: init out

# Link
out: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(CXXFLAGS) $(SFML_AND_SDL2) $(SANITIZER)

# Compile source files
obj/%.o: src/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(SANITIZER)

# Clean
clean:
	rm -rf out obj/*.o 

init:
	mkdir -p obj