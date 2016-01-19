# OBJS specifies which files on which compilation depends
OBJS = fire.c

# Compiler flags
COMPILER_FLAGS 	= -Wall -pedantic
DFLAGS			= -DDEBUGFLAG
LINKER_FLAGS 	= -lSDL2 #-lSDL2_image

# Commands
CC = gcc
RM = rm -f

# OBJ_NAME specifies the name of our executable
OBJ_NAME = fire

# This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(DFLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	$(RM) *.o $(OBJ_NAME)
