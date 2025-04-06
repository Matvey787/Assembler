# Compiler and flags
CXX = g++
SANITIZER = 
SFML_AND_SDL2 = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lSDL2 -lSDL2_mixer
CXXFLAGS = -I inc -DON_STAT -mavx -D _DEBUG -std=c++17 -O3 -Wall -Wextra -Weffc++                 \
# -pg - for getting statistics of every function
# -Wfloat-equal 
# Object files
OBJECTS = obj/profiler.o obj/buterbrod_spd3trueIntr.o 

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