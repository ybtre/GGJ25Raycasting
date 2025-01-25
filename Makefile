OBJS            = ./src/renderer/*.c

COMPILER        = gcc

INCLUDE_PATHS   = -Isrc/include/

LIBRARY_PATHS   = -Lsrc/lib/

COMPILER_FLAGS  = -Wextra -g -O0 

LINKER_FLAGS    = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf

EXE_NAME        = ./raycast



bmac:
	gcc -std=c99 $(shell find ./src -type f -name *.c) $(shell sdl2-config --cflags --libs) -o raycast

bwin: $(OBJS)
	$(COMPILER) $(OBJS) -o $(EXE_NAME) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) 

run:
	./raycast

clean:
	rm raycast
