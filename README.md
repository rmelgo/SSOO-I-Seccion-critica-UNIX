# Sección crítica con procesos UNIX

# - Introducción

Proyecto realizado en la asignatura de SSOO I del grado de Ingenieria Informática de la Universidad de Salamanca. El enunciado del proyecto se encuentra en el siguiente enlace:  

http://avellano.usal.es/~labssoo/pract109.htm
  
El principal objetivo de este proyecto es la realización de un programa en C que simule la entrada y salida de procesos en una sección crítica. 
La particularidad de este programa radica en que el único mecanismo de comunicación entre procesos permitido para implementar la sección crítica es el envio de señales.

# - Comentarios sobre el entorno de ejecución

Para ejecutar este programa se requerira de una distribución del Sistema Operativo **GNU/Linux**.    

Para poder compilar correctamente el programa, se deberá tener instalada una version del compilador gcc o similar. En el caso de no tener gcc, se puede instalar facilmente con el siguiente comando:

```sudo apt install gcc```

# - Modos de ejecución

Existen 2 modos de funcionamiento:

En el modo de funcionamiento ***normal***, los procesos entran y salen directamente en la sección crítica sin realizar ningún tipo de espera. Tampoco se imprime por pantalla el PID del proceso que entra/sale de la sección crítica.

En el modo de funcionamiento de ***depuración***, los procesos entran y salen de la sección crítica realizando pequeñas pausas. En este caso, los procesos que entran/salen de la sección crítica imprimen su PID.

# - Parámetros de ejecución

Para ejecutar el programa es necesario proporcionar 2 argumentos. 

El primer argumento se trata de un número entero comprendido entre 3 y 33, el cual hace referencia al número de procesos que participan en la práctica.

El segundo argumento es opcional. Si el segundo argumento es la palabra debug, el programa se ejecuta en modo de ***depuración***. Si el segundo parametro viene vacio, el programa se ejecuta en modo ***normal***.

Si los parámetros introducidos no respetan las reglas anteriores, el programa lo detectará, informará al usuario y acabará.

# - Pasos necesarios para ejecutar el programa

**Paso 1: Compilar el programa**  

Para ello se debe introducir el siguiente comando:    

```gcc lallevas.c -o lallevas```

Tras ejecutar este comando, se generará un fichero ejecutable llamado *lallevas*

**Paso 2: Ejecutar el programa**  

Para ello se debe introducir el siguente comando:    

```./lallevas 6```

Tras ejecutar este comando, el programa se habra ejecutado correctamente, siempre y cuendo se hayan introducido los argumentos correspondientes.

# - Ejemplo de ejecución

En la siguiente imagen, se muestra un ejemplo del uso y funcionamiento del programa en el modo de depuración:    

![Ejemplo ejecucion lallevas 1](https://github.com/rmelgo/SSOO-I-Seccion-critica-UNIX/assets/145989723/a70d4077-3c47-4e50-b2bb-597b33f2ff75)

En la siguiente imagen, se muestra un ejemplo del uso y funcionamiento del programa en el modo normal:    

![Ejemplo ejecucion lallevas 2](https://github.com/rmelgo/SSOO-I-Seccion-critica-UNIX/assets/145989723/3ffe78dc-5954-42fa-815f-472b41c12d82)
