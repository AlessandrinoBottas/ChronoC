# Compiler and flags
CC      = clang
CFLAGS  = -Wall -O2 -std=c11 -Iinclude
LIBS    = -lncurses -lpthread

# Targets and files
TARGET  = ChronoC
SRCDIR  = src
INCDIR  = include

SRCS    = $(wildcard $(SRCDIR)/*.c)
OBJS    = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link all objects to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile each .c file into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(TARGET) $(SRCDIR)/*.o

.PHONY: all clean
