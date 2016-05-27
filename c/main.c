#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include "mono.h"

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("No se parametrizaron los monos\n");
		printf("Modo de uso: main.c <monos_izquierda> <monos_derecha>\n");
	} else {
		int *data;
		int shm_fd = shm_open("/shm_monos", O_RDWR | O_CREAT, 0777);
		ftruncate(shm_fd, sizeof(int));
		data = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
		int izquierda;
		int derecha;
		for (izquierda = 0; izquierda < atoi(argv[1]); izquierda++)
			system(MONO_IZQUIERDA);
		for (derecha = 0; derecha < atoi(argv[2]); derecha++) 
			system(MONO_DERECHA);
	}
	return 0;
}