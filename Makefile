TARGET       = bin/chicken
INCLUDE_PATH = -I/usr/include/ -Ilib/CGF/include -Ilib/CGF/include/tmxloader -Ilib/CGF/include/pugixml
LIB_PATH     = -Llib/CGF/lib -Llib
CXX          = clang++
CXX_FLAGS    = -std=c++11 -Wall -Wextra $(INCLUDE_PATH)

LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lBox2D -lCGF -lz

SOURCES = $(wildcard source/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIB_PATH) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(SOURCES:.cpp=.o) $(TARGET)
