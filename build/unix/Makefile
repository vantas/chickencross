TARGET = chicken
INCLUDE_PATH = -I../../lib/CGF/include 
LIB_PATH = -L../../lib/CFG/lib -L../../lib
CXX    = clang++
CXX_FLAGS = -std=c++11 -Wall -Wextra $(INCLUDE_PATH)

#SFML_LIBS_ALL = -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system

SOURCE_DIR = ../../source

all: main.o MenuState.o PlayState.o
	$(CXX) main.o MenuState.o PlayState.o $(LIB_PATH) $(SFML_LIBS) -lCGF-Linux -o ../../bin/$(TARGET)

main.o: $(SOURCE_DIR)/main.cpp
	$(CXX) $(CXX_FLAGS) -c $(SOURCE_DIR)/main.cpp

MenuState.o: $(SOURCE_DIR)/MenuState.cpp
	$(CXX) $(CXX_FLAGS) -c $(SOURCE_DIR)/MenuState.cpp

PlayState.o: $(SOURCE_DIR)/PlayState.cpp
	$(CXX) $(CXX_FLAGS) -c $(SOURCE_DIR)/PlayState.cpp

clean:
	rm -rf *.o
	rm $(TARGET)
