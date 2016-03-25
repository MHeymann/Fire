# OBJS specifies which files on which compilation depends
OBJDIR = objs
OBJS = $(OBJDIR)/fire.o
EXES = fire

# Compiler flags
CFLAGS 	= -Wall -pedantic
DFLAGS			= -DDEBUGFLAG
LDFLAGS 	= -lSDL2 #-lSDL2_image

# Commands
CC = gcc
RM = rm -f
COMPILE = $(CC) $(CFLAGS) $(DFLAGS)


# This is the target that compiles our executable
all: $(EXES)

fire: $(OBJS)
	$(COMPILE) $^ $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: src/%.c 
	$(COMPILE) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o $(EXES)
