all: main mono

main: main.o
	gcc main.o -o main

main.o: main.c
	gcc -c main.c -o main.o

mono: mono.o
	gcc mono.o -o mono -pthread

mono.o: mono.c
	gcc -c mono.c -o mono.o

clean:
	rm mono main *.o
	rm dev/shm/sem.derecha dev/shm/sem.izquierda
