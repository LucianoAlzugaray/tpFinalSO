#!/usr/bin/env python

from kivy.support import install_twisted_reactor
install_twisted_reactor()

import os
import signal
import sys
import subprocess
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.factory import Factory
from leer_struct import buscar_estructura
from threading import Thread

TMensaje = buscar_estructura(
    './c/comm.h',
    'TMensaje'
)
from twisted.internet import protocol, reactor

class EscuchaC(protocol.Protocol):
    def __init__(self, app):
        self.app = app
    def datagramReceived(self, data, direccion):
        print(len(data))
        m = TMensaje()
        m.unpack(data)
        self.app.llego_mensaje(m)

print TMensaje
class Ventana(Widget):
    pass


class VisualApp(App):

    procesos = {}

    def build(self):
        reactor.listenUDP(2016, EscuchaC(self))
        return Ventana()

    def matar_a_todos(self):
        area_visualizacion = self.root.ids.vis
        for pid, grafico in self.procesos.iteritems():
            try:
                os.kill(pid, signal.SIGTERM)
            except:
                pass
            area_visualizacion.remove_widget(grafico)
            self.procesos = {}
    def compilar(self):
        subprocess.call(
            'gcc ./c/comm.c ./c/prog_a.c -o ./c/prog_a &&'
            'gcc ./c/comm.c ./c/prog_b.c -o ./c/prog_b &&'
            'echo "Compilacion OK"', shell=True)
    def ejecutar(self, comando):
        hilo = Thread(target=subprocess.call,
        args=(comando,))
        hilo.start()
        #subprocess.call(comando)
    def agregar_coso(self):
        #import pdb; pdb.set_trace()
        coso = Factory.Coso()
        self.root.ids.vis.add_widget(coso,
        )

    def llego_mensaje(self, m):
        '''
        m es del tipo TMensaje o sea que tiene
        todo lo que escibimos en c/comm.h adentro
        del struct {} TMensaje
        '''
        area_visualizacion = self.root.ids.vis
        if not m.pid in self.procesos:

            grafico = Factory.Proceso(
                # Agregar un elemento a la pantalla
                x=area_visualizacion.x + m.x,
                y=area_visualizacion.y + m.y,
                source=m.imagen.strip('\x00')
            )
            area_visualizacion.add_widget(grafico)

            self.procesos[m.pid] = grafico
        else:

            proceso = self.procesos[m.pid]
            if m.estado == -1:
                area_visualizacion.remove_widget(
                    proceso
                )
                del self.procesos[m.pid]

            proceso.x = m.x +  area_visualizacion.x
            proceso.y = m.y +  area_visualizacion.y
            proceso.source = m.imagen.strip('\x00')
if __name__ == '__main__':
    VisualApp().run()
