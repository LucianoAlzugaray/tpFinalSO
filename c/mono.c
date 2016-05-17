#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "comm.h"
#include "mono.h"

void creado(char*, TMensaje);
void pasar(char*, TMensaje);
void morir(TMensaje);

int main(int argc, char *argv[]){
	//Inicia los futuros mensajes.
	TMensaje m;
	iniciar(argc, argv, &m);
	strcpy(m.imagen, "mono.png");
	m.pid = getpid();
	
	//Inicia los semaforos y memoria compartida.
	sem_t *s1,*s2;
	int *data;
	int shm_fd = shm_open("/shm_monos", O_RDWR | O_CREAT, 0777);
	ftruncate(shm_fd, sizeof(int));
	data = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
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
	sem_wait(s1);
	*data++;
	creado(argv[1], m);
	if (*data == 1)
		sem_wait(s2);
	sem_post(s1);
	pasar(argv[1],m);
	sem_wait(s1);
	*data--;
	if (*data == 0)
		sem_post(s2);
	sem_post(s1);
	morir(m);
	return 0;
}

void creado(char *monte, TMensaje m){
	printf("Soy el mono %d y fui creado. Estoy en la %s\n", getpid(),monte);
	if(strcmp(monte,"izquierda") == 0) {
		m.x = X_PUNTO_INICIAL_IZQUIERDA;
		m.y = Y_PUNTO_INICIAL_IZQUIERDA;
		enviar(&m);		
	} else {
		m.x = X_PUNTO_INICIAL_DERECHA;
		m.y = Y_PUNTO_INICIAL_DERECHA;
		enviar(&m);		
	}
	
}

void pasar(char *monte, TMensaje m) {
	double vector_x;
	double vector_y;
	int modulo;
	printf("Soy el mono %d y estoy pasando\n", getpid());
	do { 
		if(strcmp(monte,"izquierda") == 0) {
			vector_x = X_PUNTO_INICIAL_DERECHA - m.x;
			vector_y = Y_PUNTO_INICIAL_DERECHA - m.y;
		} else {
			vector_x = X_PUNTO_INICIAL_IZQUIERDA - m.x;
			vector_y = Y_PUNTO_INICIAL_IZQUIERDA - m.y;
		}

		modulo = sqrt( pow(vector_x, 2) + pow(vector_y, 2) ); 

		vector_x /= modulo; //se divide el vector por su módulo.
		vector_y /= modulo; 

		m.x = (int)( m.x + vector_x );
		m.y = (int)( m.y + vector_y );

		enviar(&m);
	} while ( (vector_x != 0) && (vector_y != 0) );
}

void morir(TMensaje m){
	printf("Soy el mono %d y termine de pasar.\n", getpid());
	m.estado = -1;
	enviar(&m);		
}
