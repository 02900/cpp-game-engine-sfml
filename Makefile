# define which compiler to use
CXX		:= g++

OUTPUT	:= sfmlgame
SFML_DIR	:= /opt/homebrew/Cellar/sfml/2.6.1

# compiler and linker flags
CXX_FLAGS	:= -O3 -std=c++20 -Wno-unused-result
INCLUDES	:= -I ./src -I ./src/imgui-1.90.1 -I ./src/imgui-sfml-2.6 -I ${SFML_DIR}/include
LDFLAGS		:= -O3 -L ${SFML_DIR}/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -framework OpenGL

# the source files for the ecs game engine
SRC_FILES	:= $(wildcard src/*.cpp src/imgui-1.90.1/*.cpp src/imgui-sfml-2.6/*.cpp)
OBJ_FILES	:= $(SRC_FILES:.cpp=.o)

# all of these targets will be made if you just type 'make'
all: $(OUTPUT)

# define the main executable requirements / command
$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# specifies how the object files are compiled from cpp files
.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

# typing 'make clean' will remove all intermediate build files
clean:
	rm -f $(OBJ_FILES) ./bin/$(OUTPUT)

# typing 'make run' will build and run the game
run: all
	./bin/$(OUTPUT)
