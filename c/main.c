#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MONOS 20

int main(int argc, char *argv[]){
	pthread_t monos[MAX_MONOS];
	if(argc != 3){
		printf("No se parametrizaron los monos\n");
		printf("Modo de uso: main.c <monos_izquierda> <monos_derecha>\n");
	} else {
		int cant_monos;
		int izquierda;
		int derecha;
		for (cant_monos = 0; cant_monos <= MAX_MONOS - 1 ; cant_monos++){
			for (izquierda = 0; izquierda < atoi(argv[1]); izquierda++){
				system("./mono izquierda");
			}
			for (derecha = 0; derecha < atoi(argv[2]); derecha++) {
				system("./mono derecha")
			}
		}
	}
	return 0;
}