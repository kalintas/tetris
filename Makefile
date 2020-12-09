CC = g++

CFLAGS = -Wall -ansi -pedantic-errors -std=gnu++11


INCLUDES =

LFLAGS =

LIBS = -lmingw32 -lSDL2main -lSDL2


SRCS = dropping_piece.cpp tetris.cpp main.cpp window.cpp

OBJS = $(SRCS:.c=.o)

MAIN = e

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
