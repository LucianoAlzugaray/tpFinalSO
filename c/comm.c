/**
 * Este archivo se compila como comm.<SO>.<Arquitectura>.o
 * En linux el binario sería comm.linux.x86_64.o en un procesador de 64bits
 */

#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0) atexit()
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "comm.h"
#define SERVER "127.0.0.1"
#define BUFLEN 512

struct sockaddr_in si_other;
int s,
    i,
    slen,
    id=0 /* Identificado de mensaje */;
char prog_name[BUFLEN];

/**
 * Cerrar el programa imprimiendo mensaje de error.
 * @param s [description]
 */
void die(char *s)
{
    perror(s);
    exit(1);
}

/**
 * Cierra socket y envía mensaje de finalización con
 * status = -1 para que processing remueva el elemento de la pantalla.
 */
void close_socket(void) {
    TMensaje m;
    m.estado = -1;
    enviar(&m);
    close(s);
}

/**
 * Inicialización de la biblioteca de comunicación
 * @param argc Argumento argc de main
 * @param argv Argumento argv de main
 * @param m    Mensaje a ser utilizado para comunicación, para realizar comunicación.
 */
void iniciar(int argc, char **argv, TMensaje *m) {
    int port = PORT;
    if (m == NULL) {
        die("El agumento TMensaje m es nulo.");
    }
    char *port_s = getenv("PORT");

    if (port_s != NULL) {
        port = atoi(port_s);
        if (port < 1024 || port > 65000) {
            port = PORT;
        }
        fprintf(stderr, "PORT no se pudo entender. Dejando valor: %d\n", port);
    }

    if (argc) {
        strncpy(prog_name, argv[0], sizeof(prog_name)-1);
    }
    slen = sizeof(si_other);
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    atexit(close_socket);
    memset((void *)m, 0, sizeof(TMensaje));
    fprintf(stderr, "Puerto UDP: %d", port);
}


void enviar(TMensaje *m) {
    m->pid = getpid();
    m->id = id++;
    strncpy(m->prog_name, prog_name, sizeof(m->prog_name)-1);
    if (sendto(s, m, sizeof(TMensaje) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }
}

