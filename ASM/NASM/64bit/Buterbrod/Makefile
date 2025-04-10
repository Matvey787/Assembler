# Compiler and flags
CXX = g++
SANITIZER = 
SFML_AND_SDL2 = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lSDL2 -lSDL2_mixer
CXXFLAGS = -I inc -DON_STAT -DTRUEAVX256 -mavx -D _DEBUG -std=c++17 -O3 -Wall -Wextra -Weffc++  \

# -DNAIVE_V
# -DSPD1_V
# -DAVX4
# -DAVX8
# -DTRUEAVX256

# -DON_GR	- switch on graphics
# -DON_STAT	- switch on statistics with profiler
# -DONPROGBAR - switch on progress bar

# -pg - for getting statistics of every function
# -Wfloat-equal 
# Object files
OBJECTS = obj/profiler.o obj/buterbrod.o 

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