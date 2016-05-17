all: main mono

main: main.o
	gcc main.o -o main -lrt

main.o: main.c
	gcc -c main.c -o main.o

mono: mono.o comm.linux.x86_64.o
	gcc mono.o comm.linux.x86_64.o -o mono -pthread -lrt -lm

mono.o: mono.c main
	gcc -c mono.c -o mono.o 

comm.linux.x86_64.o: comm.c
	gcc -c comm.c -o comm.linux.x86_64.o 

clean:
	rm mono main *.o
	rm dev/shm/sem.derecha dev/shm/sem.izquierda dev/shm/shm_monos
