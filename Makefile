INCLUDES = -I ./include
FLAGS = -g

OBJECTS = ./build/chip48memory.o ./build/chip48stack.o

all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip48memory.o: src/chip48memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip48memory.c -c -o ./build/chip48memory.o

./build/chip48stack.o: src/chip48stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip48stack.c -c -o ./build/chip48stack.o

clean:
	del build\*.o
	del bin\main.exe