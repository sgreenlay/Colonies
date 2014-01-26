CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=

SOURCES=main.c engine/engine.c engine/graphics.c engine/input.c engine/util.c engine/clock.c game/game.c
OBJECTS=$(SOURCES:.c=.o)
EXE=ggj14

INCLUDEPATHS=-I/usr/local/include/SDL2/ -Iengine/inc -Igame/inc
LIBPATHS=-L/usr/local/lib/

LIBS=-lSDL2 -lSDL2_image

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBPATHS) -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDEPATHS) $< -o $@

clean:
	rm *.o */*.o ggj14*