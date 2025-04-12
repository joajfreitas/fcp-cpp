CC = ccache g++

C_SOURCES := $(shell find src -type f -name "*.c" | grep -v "main" | cut -d"/" -f2-)
CPP_SOURCES += $(shell find src -type f -name "*.cpp" | grep -v "main" | cut -d"/" -f2-)
CPP_TESTS := $(shell find tests -type f -name "*.cpp" | cut -d"/" -f2-)

OBJDIR := src/
TESTDIR := tests/

OBJECTS := $(addprefix $(OBJDIR), $(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(OBJDIR), $(CPP_SOURCES:.cpp=.o))

TESTS := $(addprefix $(TESTDIR), $(CPP_TESTS:.cpp=.o))

INCLUDES := src

# Optimization / Debug flags
# OPT := -Ofast
OPT := -g

# Build rules
%.o: %.c
	gcc -I$(INCLUDES) $(OPT) -c -fPIC $< -o $@

%.o: %.cpp
	$(CC) -I$(INCLUDES) $(OPT) -c -fPIC $< -o $@

fcp-cpp.so: $(OBJECTS)
	$(CC) $(OPT) $^ -shared -o $@

# Format all C/C++ source and header files, excluding some patterns
clang-format:
	@ find "$(shell pwd)"/src "$(shell pwd)"/tests -not -name "signal_parser*" \
		| grep --regexp='.*\.\(cpp\|hpp\|c\|h\)$$' \
		| xargs -P $(shell nproc) -I "{}" sh -c 'echo "CLANG-Format -> {}" && clang-format -i -style=Webkit "{}" || true'

# Optional: to check if everything is formatted (CI/linting)
clang-format-check:
	@ find "$(shell pwd)"/src "$(shell pwd)"/tests -not -name "signal_parser*" \
		| grep --regexp='.*\.\(cpp\|hpp\|c\|h\)$$' \
		| xargs -P $(shell nproc) -I "{}" sh -c 'echo "CLANG-Format [CHECK] -> {}" && clang-format --dry-run -Werror -style=Webkit "{}"'


install:
	@mkdir -p /usr/lib/fcp-cpp
	@mkdir -p /usr/include/fcp-cpp
	@cp fcp-cpp.so /usr/lib/fcp-cpp
	@cp src/*.h src/*.hpp /usr/include/fcp-cpp
	@ldconfig -n -v /usr/lib

clean:
	rm -rf *.so
	rm -rf src/*.o
	rm -rf tests/*.o

test: $(OBJECTS) $(TESTS)
	@echo "Linking tests..."
	$(CC) $^ -o test_exec
	@echo "Running tests..."
	@./test_exec

