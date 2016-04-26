#ifndef _comm_h_
#define _comm_h_

/*
Definiciones globales
 */
#define PORT 2016

/**
 * Modificar esta estructura para mostrar información útil
 */
typedef struct {
    char prog_name[20];
    int pid;
    int id;
    int estado;
    /* Información extra */
    char imagen[80];
    int x;
    int y;
} TMensaje;


/**
 * Iniciar la comunicación con Processing.
 * Debe ser llamado antes de utilizar enviar.
 */
void iniciar(int argc, char **argv, TMensaje *m);
/**
 * Enviar un mensaje a Processing
 * @param m mensaje a ser enviado. Este mensaje está definido en el tipo TMensaje que
 *          puede ser modificado para agregar información útil de acuerdo al problema
 *          que se esté simulando.
 */
void enviar(TMensaje *m);

#endif
