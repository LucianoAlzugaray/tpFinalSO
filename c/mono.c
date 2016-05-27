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

int valor, bloqueo = 0;
TMensaje m;

int main(int argc, char *argv[]){
	//Inicia los futuros mensajes.

	iniciar(argc, argv, &m);
	strcpy(m.imagen, "mono.png");
	m.pid = getpid();
	
	//Inicia los semaforos y memoria compartida.
	sem_t *s1,*s2, *sem_general;
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
	if ((sem_general = sem_open("/sem_general", O_CREAT, 0700,1)) == SEM_FAILED)
		perror("sem_open(sem_general)");

	//Se creo el mono. Manda el mensaje correspondiente.
	creado(argv[1], m);
	sem_wait(sem_general);
	sem_getvalue(s1,&valor);
	if(valor == 0){
		printf("Soy el mono %d y estoy bloqueando\n",getpid() );
		sem_post(sem_general);
		bloqueo = 1;
	}
	sem_wait(s1);
	if (bloqueo)
		sem_wait(sem_general);
	sleep(1);
	*data = *data + 1;
	if (*data == 1)
		sem_wait(s2);
	sem_post(s1);
	sem_post(sem_general);
	pasar(argv[1],m);
	sem_wait(s1);
	*data = *data - 1;
	if (*data == 0)
		sem_post(s2);
	sem_post(s1);
	morir(m);
	return 0;
}

void creado(char *monte, TMensaje m){
	printf("Soy el mono %d y fui creado. Estoy en la %s\n. Mensaje de x:%d mensaje de y: %d", getpid(),monte, m.x, m.y);
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
	int x_actual;
	int y_actual;
	int vector_x;
	int vector_y;
	int modulo;
	if(strcmp(monte,"izquierda") == 0) {
		x_actual = X_PUNTO_INICIAL_IZQUIERDA;
		y_actual = Y_PUNTO_INICIAL_IZQUIERDA;
	} else {
		x_actual = X_PUNTO_INICIAL_DERECHA;
		y_actual = Y_PUNTO_INICIAL_DERECHA;
	}

	printf("Soy el mono %d y estoy pasando\n", getpid());
	while(1){  
		usleep(100000);
		if(strcmp(monte,"izquierda") == 0) {
			vector_x = X_PUNTO_INICIAL_DERECHA - x_actual;
			vector_y = Y_PUNTO_INICIAL_DERECHA - y_actual;
		} else {
			vector_x = X_PUNTO_INICIAL_IZQUIERDA - x_actual;
			vector_y = Y_PUNTO_INICIAL_IZQUIERDA - y_actual;
		}

		modulo = sqrt( pow(vector_x, 2) + pow(vector_y, 2) ); 

		if (modulo > 0){
			vector_x /= modulo; //se divide el vector por su módulo.
			vector_y /= modulo; 
		}
		else
			break;
		x_actual = x_actual + vector_x;
		y_actual = y_actual + vector_y;
		
		m.x = x_actual;
		m.y = y_actual;

		printf("x = %d ---- y = %d, vector_x : %d vector_y: %d \n", m.x, m.y, vector_x, vector_y);
		enviar(&m);
	}

}

void morir(TMensaje m){
	printf("Soy el mono %d y termine de pasar.\n", getpid());
	m.estado = -1;
	enviar(&m);		
}
