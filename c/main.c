#include <stdio.h>
#include <stdlib.h>

#define MAX_MONOS 20

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("No se parametrizaron los monos\n");
		printf("Modo de uso: main.c <monos_izquierda> <monos_derecha>\n");
	} else {
		int izquierda;
		int derecha;
		for (izquierda = 0; izquierda < atoi(argv[1]); izquierda++)
			system("./mono izquierda");
		for (derecha = 0; derecha < atoi(argv[2]); derecha++) 
			system("./mono derecha");
	}
	return 0;
}