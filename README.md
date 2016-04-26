# Ejemplo de visulización con Kivy

Ejemplo desarrollado el 25/4/2016 en clase a partir del ejemplo del [tutorial
del kivy]():

```python
from kivy.app import App
from kivy.uix.widget import Widget


class PongGame(Widget):
    pass


class PongApp(App):
    def build(self):
        return PongGame()


if __name__ == '__main__':
    PongApp().run()
```

Luego se experimentó con el lenguaje kv de definción de interfaces.

Se aregaron botones con diferentes callbacks en la aplicación utlizando la referencia `app`.

Se lanzaron procesos utilizando el módulo `subprocess.call` y se utilizaron hilos para hacerlo concurrente `threading.Thread`.

Se agregó Twisted para recibir datos a través de UDP.

Luego se comunicó la recepción de mensajes con el área de visulización.
