# encoding: utf-8
import re
from cstruct import CStruct, LITTLE_ENDIAN


def buscar_estructura(archivo=None, nombre=None):
    """
    Busca la definición de una estructura de C en un archivo.
    Recorre el archivo de atrás hacia adelante.
    """
    with open(archivo) as fp:
        lineas = reversed(fp.readlines())
    contenido = []
    encontrado = False
    FIN = re.compile(r'\}\s*%s\s*;' % nombre)
    INICIO = '{'
    for i, linea in enumerate(lineas):

        if not encontrado:
            if FIN.search(linea):
                encontrado = True  # Econtramos cierre
        else:
            if INICIO in linea:
                break  # Econtramos apertura
            contenido.append(linea)
    contenido.reverse()
    contenido = ''.join(contenido)
    tipo_de_dato = type(
        nombre,
        (CStruct, ),
        {
            '__byte_order__': LITTLE_ENDIAN,
            '__struct__': contenido
        }
    )

    return tipo_de_dato

if __name__ == "__main__":
    from pprint import pprint as pp
    pp(buscar_estructura(archivo='./c/comm.h', nombre='TMensaje'))