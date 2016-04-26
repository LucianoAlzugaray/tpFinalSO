#include "comm.h"
#include <string.h> // strncpy
#include <unistd.h>


int main(int argc, char **argv)
{
    TMensaje m;
    int count = 10;

    iniciar(argc, argv, &m);

    while(count-- > 0)
    {
        // Enviar el mensaje
        m.x = count;
        m.y = count +1;
        m.pid = getpid();
        enviar(&m);
        usleep(500000);
    }
    // Avisa a Processing que se va
    m.estado = -1;
    enviar(&m);

    return 0;
}
