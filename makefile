#How to Use It
#	1.	Save the Makefile in the same directory as your .c and .h files.
#	2.	Open the terminal in your Codespace.
#	3.	Run at bash:
#		make 
#  This will compile the .c files and generate an executable named program.
#.  4.	Run the program at bash
#.   ./program
#
#	5.	To clean up the generated object files and executable, run at bash
#.      make clean
#

# Compiler
# CC specifies the compiler (gcc in this case).
CC = gcc

# Compiler flags 
# CFLAGS are the flags passed to the compiler (-Wall enables all warnings; -g adds debug info).
CFLAGS = -Wall -g

# Executable name
TARGET = program

# Source files
SRCS = main.c my_btree.c my_stack.c my_queue.c

# Object files (replace .c with .o in SRCS)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
# To clean up the generated object files and executable, run: 
#   make clean
clean:
	rm -f $(OBJS) $(TARGET)