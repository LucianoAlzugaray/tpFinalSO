#include "comm.h"
#include <string.h> // strncpy
#include <unistd.h>


int main(int argc, char **argv)
{
    TMensaje m;
    int count = 10;

    iniciar(argc, argv, &m);
    strcpy(m.imagen, "pelota.png");
    while(count-- > 0)
    {
        // Enviar el mensaje
        m.x = count * 10;
        m.y = count * 10+1;
        m.pid = getpid();
        enviar(&m);
        usleep(500000);
        if ( count == 5) {
            strcpy(m.imagen, "pantera.png");
        }
    }

    return 0;
}
