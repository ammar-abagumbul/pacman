CC = g++

CFLAGS = -pedantic-errors -std=c++11 -Wall

DEPS = include/GameObject.h include/Game.h include/Player.h include/Direction.h include/Map.h \
	include/Ghost.h include/Direction.h include/Navigator.h

LDLIBS = -lncurses

OBJ = Main.o Game.o Player.o Ghost.o Navigator.o Map.o

.PHONY: all clean move

all: game

game: $(OBJ)
	$(CC) $(CFLAGS) -o game $(OBJ) $(LDLIBS)

Main.o: src/Main.cpp $(DEPS)
	$(CC) $(CFLAGS) -c src/Main.cpp

Game.o: src/Game.cpp $(DEPS)
	$(CC) $(CFLAGS) -c src/Game.cpp

Player.o: src/Player.cpp $(DEPS)
	$(CC) $(CFLAGS) -c src/Player.cpp

Ghost.o: src/Ghost.cpp $(DEPS)
	$(CC) $(CFLAGS) -c src/Ghost.cpp

Navigator.o: src/Navigator.cpp $(DEPS)
	$(CC) $(CFLAGS) -c src/Navigator.cpp

Map.o: src/Map.cpp $(DEPS)
	$(CC) $(CFLAGS) -c src/Map.cpp

clean:
	rm -rf ./obj/*
	rm -f game

move: 
	mkdir -p obj
	mv $(OBJ) ./obj

build_and_move: all move

.DEFAULT_GOAL = build_and_move