TARGET 		   = ../../bin/libCGF.dylib
INCLUDE_PATH = -Iinclude -Iinclude/tmxloader -Iinclude/pugixml
CXX          = clang++
CXX_FLAGS    = -std=c++11 -Wall -Wextra $(INCLUDE_PATH)
LIB_FLAGS    = -dynamiclib
UNAME_S      = $(shell uname -s)
LIBS         = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lBox2D -lz

ifeq ($(UNAME_S),Linux)
	TARGET     = ../../bin/libCGF.so
	CXX_FLAGS  += -fPIC
	LIB_FLAGS  = -shared
endif

SOURCES  = $(wildcard source/*.cpp)
SOURCES += $(wildcard source/pugixml/*.cpp)
SOURCES += $(wildcard source/tmxloader/*.cpp)
OBJECTS  = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CXX) $(LIB_FLAGS) $(OBJECTS) $(LIB_PATH) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(SOURCES:.cpp=.o) $(TARGET)
