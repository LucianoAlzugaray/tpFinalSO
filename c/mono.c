#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "comm.h"
#include "mono.h"

void creado(*char);
void pasar(void);
void morir(void);

extern int numero_monos;

int main(int argc, char *argv[]){
	//Inicia los futuros mensajes.
	Tmensaje m;
	iniciar(argc, argv, &m);
	strcpy(m.imagen, "mono.png");
	m.pid = getpid();
	//Inicia los semaforos
	sem_t *s1,*s2;

	//Asigna los semaforos segun si el mono esta a la izquierda o derecha.
	if(strcmp(argv[1], "izquierda") == 0){
		if((s1 = sem_open("/izquierda", O_CREAT, 0700, 1)) == SEM_FAILED)
			perror("sem_open(s1)");
		if((s2 = sem_open("/derecha", O_CREAT, 0700, 1)) == SEM_FAILED)
			perror("sem_open(s2)");
	} else {
		if((s1 = sem_open("/derecha", O_CREAT, 0700, 1)) == SEM_FAILED)
			perror("sem_open(s1");
		if((s2 = sem_open("/izquierda", O_CREAT, 0700, 1)) == SEM_FAILED)
			perror("sem_open(s2)");
	}

	//Se creo el mono. Manda el mensaje correspondiente.
	creado(argv[1]);
	numero_monos++;

	if (numero_monos == 1)
		sem_wait(s2);
	sem_post(s1);
	pasar();
	sem_wait(s1);
	numero_monos--;
	if (numero_monos == 0)
		sem_post(s2);
	sem_post(s1);
	morir();
	return 0;
}

void creado(*char monte){
	if(strcmp(monte,"izquierda") == 0) {
		m.x = 10;
		m.y = 10;
		enviar(&m);		
	} else {
		m.x = 20;
		m.y = 20;
		enviar(&m);		
	}
	
}

void pasar(void) {
		m.x = 30;
		m.y = 30;
		enviar(&m);		
}

void morir(void){
	m.x = 40;
	m.y = 40;
	enviar(&m);		
}