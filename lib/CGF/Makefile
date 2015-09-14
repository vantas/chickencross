TARGET 		   = lib/libCGF.dylib
INCLUDE_PATH = -Iinclude -Iinclude/tmxloader -Iinclude/pugixml
CXX          = clang++
CXX_FLAGS    = -std=c++11 -Wall -Wextra $(INCLUDE_PATH)

LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lBox2D -lz

SOURCES  = $(wildcard source/*.cpp)
SOURCES += $(wildcard source/pugixml/*.cpp)
SOURCES += $(wildcard source/tmxloader/*.cpp)
OBJECTS  = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CXX) -dynamiclib $(OBJECTS) $(LIB_PATH) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(SOURCES:.cpp=.o) $(TARGET)
