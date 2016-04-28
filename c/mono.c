#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "comm.h"

void pasar(void);

int numero_monos;

int main(int argc, char *argv[]){
	sem_t *s1,*s2;
	if(strcmp(argv[1], "izquierda") == 0){
		if((s1 = sem_open("/izquierda", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s1)");
		if((s2 = sem_open("/derecha", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s2)");
		printf("Soy el mono %d y voy a la izquierda\n", getpid());

	} else {
		if((s1 = sem_open("/derecha", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s1");
		if((s2 = sem_open("/izquierda", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s2)");
		printf("Soy el mono %d y voy a la derecha\n", getpid());

	}

		sem_wait(s1);
		numero_monos ++;
		if (numero_monos == 1)
			sem_wait(s2);
		sem_post(s1);
		pasar();
		sem_wait(s1);
		printf("Pase\n");
		numero_monos--;
		if (numero_monos == 0)
			sem_post(s2);
		sem_post(s1);

	return 0;
}

void pasar(void){
	printf("Soy el mono %d y estoy pasando\n",getpid());
	sleep(5);
}