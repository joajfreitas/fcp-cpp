CC=g++

C_SOURCES := $(shell find -L src -type f -name "*.c" | grep -v "main" | cut -d"/" -f2-)
CPP_SOURCES += $(shell find -L src -type f -name "*.cpp" | cut -d"/" -f2-)

OBJDIR := src/

OBJECTS := $(addprefix $(OBJDIR), $(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(OBJDIR), $(CPP_SOURCES:.cpp=.o))

OPT := -Ofast
#OPT := -g

%.o: %.cpp
	$(CC) $(OPT) -c -fPIC  $<  -o $@

fcp-cpp.so: $(OBJECTS)
	$(CC) $(OPT) $^ -shared -o $@

format:
	clang-format -i -style=Webkit *.c *.cpp || TRUE

install:
	@mkdir -p /usr/lib/fcp-cpp
	@mkdir -p /usr/include/fcp-cpp
	@cp fcp-cpp.so /usr/lib/fcp-cpp
	@cp *.h *.hpp /usr/include/fcp-cpp
	@ldconfig -n -v /usr/lib

clean:
	rm -rf *.so
	rm -rf **/*.o *.o
