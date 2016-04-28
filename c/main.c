#include <stdio.h>
#include <stdlib.h>
#include "mono.h"

extern int numero_monos;

int main(int argc, char *argv[]){
	numero_monos = 0;
	if(argc != 3){
		printf("No se parametrizaron los monos\n");
		printf("Modo de uso: main.c <monos_izquierda> <monos_derecha>\n");
	} else {
		int izquierda;
		int derecha;
		for (izquierda = 0; izquierda < atoi(argv[1]); izquierda++)
			system(MONO_IZQUIERDA);
		for (derecha = 0; derecha < atoi(argv[2]); derecha++) 
			system(MONO_DERECHA);
	}
	return 0;
}