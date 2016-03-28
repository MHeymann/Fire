bin_PROGRAMS = fire
fire_SOURCES = fire.c
AM_CFLAGS = -Wall -pedantic
DBGFLAGS = -DDEBUG
SDL_FLAG = -lSDL2

fire$(EXEEXT): fire.o
	$(CC) $(CFLAGS) $(DBGFLAGS) $^ $(SDL_FLAG) -o fire
