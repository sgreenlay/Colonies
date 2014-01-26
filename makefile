CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=

GAME_INCLUDES=-Igame/inc -Iassets
GAME_SOURCES= game/game.c game/planet.c game/ship.c assets/sprite_mapping.c

ENGINE_INCLUDES=-Iengine/inc
ENGINE_SOURCES= main.c engine/engine.c engine/graphics.c engine/input.c engine/util.c engine/clock.c

SOURCES=$(ENGINE_SOURCES) $(GAME_SOURCES)
OBJECTS=$(SOURCES:.c=.o)
EXE=ggj14

INCLUDEPATHS=-I/usr/local/include/SDL2/ $(ENGINE_INCLUDES) $(GAME_INCLUDES)
LIBPATHS=-L/usr/local/lib/

LIBS=-lSDL2 -lSDL2_image

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBPATHS) -o $@ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDEPATHS) $< -o $@

clean:
	rm *.o */*.o ggj14*