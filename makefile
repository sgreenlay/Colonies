CC=gcc
CFLAGS=-c -Wall
LDFLAGS=

SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXE=ggj14

INCLUDEPATHS=-I/usr/local/include/SDL2/
LIBPATHS=-L/usr/local/lib/

LIBS=-lSDL2

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBPATHS) -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDEPATHS) $< -o $@

clean:
	rm *.o ggj14*