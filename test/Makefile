CC:=clang++

C_SOURCES:=$(shell find -L . -type f -name "*.c" | cut -d"/" -f2-)
CPP_SOURCES+=$(shell find -L . -type f -name "*.cpp" | cut -d"/" -f2-)

OBJDIR:=obj/
OBJECTS := $(addprefix $(OBJDIR), $(C_SOURCES:.c=.o))
OBJECTS += $(addprefix $(OBJDIR), $(CPP_SOURCES:.cpp=.o))

test: $(OBJECTS)
	$(CC) -std=c++17 $^ -o test

$(OBJDIR)%.o: %.c
	@ mkdir -p `dirname $@`
	$(CC) -std=c++17 -c $< -o $@

$(OBJDIR)%.o: %.cpp
	@ mkdir -p `dirname $@`
	$(CC) -std=c++17 -c $< -o $@

clean:
	rm -rf $(OBJDIR) test

-include $(OBJECTS:.o=.d)

