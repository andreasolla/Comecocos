# Comecocos
El juego se basa en una versión simplificada del Comecocos tradicional, con la variación de que se puede ver también en primera persona para hacerlo más interesante. Además, sólo se puede ver lo que llega a alumbrar el foco situado en el avatar (así llamaremos al comecocos principal con el que juega el usuario). El tablero se basa en un laberinto estructurado de setos, entre los que se esconden diversas frutas, y en cuyos pasillos hay otros comecocos que se deben evitar.

El programa está desarrollado con VisualStudio en OpenGl.

## Objetivo
El objetivo del juego es coger todas las frutas (4) sin ser alcanzado por los demás comecocos, en cuyo caso se pierde y finaliza la partida. Las frutas y los comecocos se sitúan en pasillos aleatorios cada vez que se incia la partida, por lo que cada intento será diferente. 


## Instrucciones
- Al iniciar el programa, se muestra una pantalla con algunas indicaciones iniciales. Para comenzar el juego se debe pulsar la tecla ``enter``.
- Al inicio, la única iluminación es la del avatar, que tiene una linterna para iluminar hacia delante. Si resulta demasiado difícil para el usuario jugar de este modo, es posible añadir una tenue luz ambiental mediante la tecla ``L``.
- Para pasar a modo primera persona se pulsa ``1``, y para volver a vista de pájaro se pulsa ``2``.
- Para cambiar el disfraz del avatar se pulsa ``3``.
- El movimiento varía en función de la vista de la siguiente forma:
  - **Vista de pájaro:** los controles son básicos, se usan las flechas para moverse en la dirección correspondiente.
  - **Vista en primera persona:** el control es más intuitivo, para avanzar se utiliza la flecha hacia arriba ``▲`` y, para retroceder la flecha hacia abajo ``▼`` (en este caso no se gira para retroceder, lo cual podría desorientar al usuario) finalmente, las flechas izquierda y derecha (``◄ ►``) se usan para girar 90º en la dirección correspondiente. Además, hay movimiento en espejo, cuando se sale del tablero por un lado se aparece por el opuesto.
- Una vez finaliza la partida y se muestra la pantalla de fin, ya sea en caso de ganar o al perder, se puede iniciar de nuevo pulsando la tecla ``enter``, al igual que al inicio. 


## A destacar
Los aspectos más destacables en cuanto a la implementación son los siguientes:
- Uso de la iluminación, con focos y variando la luz ambiental.
- Uso de diferentes texturas para el suelo, las paredes y las frutas, así como para las pantallas de inicio y fin. Además, se puede cambiar la textura del avatar variando entre 3 diferentes.
- Variación del funcionamiento de las teclas en función del modo de juego (primera persona/vista de pájaro).
- Aplicación de transformaciones para el movimiento del avatar, así como para las frutas (giran en su sitio y hacen un movimiento oscilante de arriba-abajo).
- Implementación de las colisiones para los choques con los otros comecocos y la recolección de frutas.
- Uso de irrKlang para la inclusión de sonidos.

## Utilización
Para la puesta en marcha del programa, tras descargar o clonar el repositorio, será necesario importarlo a un proyecto de Visual Studio, para compilarlo y ejecutarlo. Las dependencias necesarias para la compilación (GLFW, glad e irrKlang) ya están incluidas.
