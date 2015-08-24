TARGET       = bin/chicken
INCLUDE_PATH = -I/usr/include/ -Ilib/CGF/include
LIB_PATH     = -Llib/CFG/lib -Llib
CXX          = clang++
CXX_FLAGS    = -std=c++11 -Wall -Wextra $(INCLUDE_PATH)

#SFML_LIBS_ALL = -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system
CGF_LIB = -lCGF-Linux

SOURCES = $(wildcard source/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIB_PATH) $(SFML_LIBS) $(CGF_LIB) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(SOURCES:.cpp=.o) $(TARGET)
