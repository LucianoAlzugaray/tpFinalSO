#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
	sem_t *s1,*s2;
	if(strcmp(argv[1], "izquierda") == 0){
		if((s1 = sem_open("/izquierda", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s1)");
		if((s2 = sem_open("/derecha", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s2)");
	} else {
		if((s1 = sem_open("/derecha", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s1");
		if((s2 = sem_open("/izquierda", O_CREAT, 0700, 0)) == SEM_FAILED)
			perror("sem_open(s2)");
	}

	return 0;
}