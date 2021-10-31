CC=ccache g++

C_SOURCES := $(shell find -L src -type f -name "*.c" | grep -v "main" | cut -d"/" -f2-)
CPP_SOURCES += $(shell find -L src -type f -name "*.cpp" | grep -v "main" | cut -d"/" -f2-)
CPP_TESTS := $(shell find -L tests -type f -name "*.cpp" | cut -d"/" -f2-)

OBJDIR := src/
TESTDIR := tests/

OBJECTS := $(addprefix $(OBJDIR), $(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(OBJDIR), $(CPP_SOURCES:.cpp=.o))

TESTS := $(addprefix $(TESTDIR), $(CPP_TESTS:.cpp=.o))

INCLUDES := src

#OPT := -Ofast
OPT := -g

%.o: %.c
	gcc -I$(INCLUDES) $(OPT) -c -fPIC $< -o $@

%.o: %.cpp
	$(CC) -I$(INCLUDES) $(OPT) -c -fPIC  $<  -o $@

fcp-cpp.so: $(OBJECTS)
	$(CC) $(OPT) $^ -shared -o $@

format:
	clang-format -i -style=Webkit  src/*.cpp

install:
	@mkdir -p /usr/lib/fcp-cpp
	@mkdir -p /usr/include/fcp-cpp
	@cp fcp-cpp.so /usr/lib/fcp-cpp
	@cp src/*.h src/*.hpp /usr/include/fcp-cpp
	@ldconfig -n -v /usr/lib

clean:
	rm -rf *.so
	rm -rf **/*.o *.o

test: $(OBJECTS) $(TESTS)
	echo $(TESTS)
	$(CC) $^ -o $@
