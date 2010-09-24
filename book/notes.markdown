* http://www.elephantsneverforget.co.uk/rg/chip8.html
* http://code.google.com/p/chip8dynarec/
* http://www.nihilogic.dk/labs/chip8/
* Atención: http://hap.samor.nl/
* http://web.archive.org/web/20040607194400/http://www.goldroad.co.uk/ (genial!!!)
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#dispcoords (joder!!!)
* http://www.pygame.org/projects/20/190/
* http://www.emutalk.net/showpost.php?p=320532&postcount=629
* http://www.emutalk.net/showpost.php?p=385589&postcount=809
* http://www.emutalk.net/showthread.php?t=19894&highlight=chip-8
* http://www.emutalk.net/showthread.php?t=21925


Hello
----


**CHIP-8**  fue un lenguaje interpretado hexadecimal introducido a mediados de 1970 por Joseph Weisbecker y orientado a la creación de videojuegos con muy pocos o escasos recursos de hardware. La idea era realmente sencilla: encontrar un espacio de desarrollo entre los lenguajes de alto nivel y el lenguaje máquina. Los lenguajes de alto nivel, por ejemplo el BASIC, eran lenguajes de propósito general y orientados al usuario final en cuanto a su aprendizaje y uso; se podía programar cualquier aplicación, pero resultaba claramente ineficiente por los límites de memoria de entonces. En el lado opuesto, el lenguaje máquina. También era posible programar lo que a no le viniera en gana, pero su curva de aprendizaje es -y ha sido siempre- tan empinada que estaba reservado a unos pocos. Veamos un ejemplo:

	10 PRINT "Hello World"
	20 RUN
	ld hl, #00AA

Joseph Weisbecker debió pensar en una solución eficiente en la que se pudiera desarrollar videojuegos consumiendos 5 ó 10 veces menos memoria que un lenguaje de alto nivel, pero asequible en el momento de escribirlos sin tener que pasar por el proceso tedioso de aprender el lenguaje de la máquina. Entre el BASIC y los nemónicos del procesador podría haber vida.

CHIP-8: un lenguaje interpretado y máquina virtual en sólo 512 bytes
-

La programación de lenguajes interpretados consistía, por entonces, en el diseño de un lenguaje máquina de alto nivel para aplicaciones específicas. Es decir, establecer un conjunto de pseudo-instrucciones en las que, cada una de ellas, realicen una tarea concreta y, por lo tanto, llamen a una subrutina de código máquina optimizada para dicha tarea. Evitar el lenguaje natural, como en el caso de BASIC, y plantearlo como instrucciones hexadecimales con un rango de variables y direcciones de memoria limitados, se conseguía reducir considerablemente las exigencias de hardware y dejando espacio de memoria suficiente para, en este caso, la creación de videojuegos.

En palabras de su autor: Escribí el juego TIC-TAC-TOE en CHIP-8. Sólo ocupaba 500 bytes frente a los 3000 de una versión en BASIC. Me quedaban 2000 bytes. (…) Esta aproximación de desarrollo mediante lenguaje intepretado hexadecimal es importante por dos motivos. Primero, reduce el coste de hardware que necesitas para tu home computing. Segundo, reduce drásticamente la cantidad de ROM que se requiere en microprocesadores orientados a controladores o videojuegos. El coste de la ROM es significativo y un factor importante para este tipo de productos

RCA COSMAC VIP
--------------

http://www.cosmacelf.com/
http://homepage.mac.com/ruske/cosmacelf/cdp1802.pdf
http://tinyurl.com/388y6lf
http://www.pdc.kth.se/~lfo/chip8/CHIP8.htm


Siguiendo esta línea de trabajo, Joseph Weisbecker optó por un RCA COSMAC VIP, un microordenador de 1977 que contenía 2048 bytes de memoria libre, salida de video, un teclado hexadecimal y un microprocesador CDP-1802 en el que pondría a prueba el CHIP-8. Veamos las siguientes cuestiones técnicas:

El intérprete ocupa 512 bytes y reside en la dirección 0000h - 01FFh de la memoria RAM.
Los programas escritos en el lenguaje CHIP-8 deben comenzar en la dirección 0200h.
El lenguaje CHIP-8 tiene un conjunto de 31 instrucciones. Cada instrucción requiere dos bytes (cuatro dígitos hexadecimales)
La Memory addressing está limitada a 4096 bytes, por lo que sólo se necesitan tres de los cuatro dígitos hexadecimales.
El número de variables se ha limitado a 16, etiquetados V0 a VF. Cada uno de ellos representa 1 byte de información de lectura y escritura.
Hay un registro especial, I, de 2 bytes de información sobre direcciones de memoria que utilizaran algunas instrucciones.
Un Timer al que puede recibir un valor del 00h a FFh por la instrucción FX15. Por ejemplo, si la variable V2 contiene el valor 0Ah, la instrucción F215 pondrá el Timer a 0Ah, el mismo que la variable V2. En este momento, el Timer restará por uno su valor 60 veces por segundo hasta alcanzar el cero. Si el valor que se le asignamos es 3Ch (60 en hexadecimal) alcanzará el valor de cero un segundo después. Este Timer es básico para generar “delays” o retrasos en el caso de los videojuegos.
Aquí vendría la tabla 

Video memoria
-------------

El área de pantalla utilizada para dibujar por la video memoria era de 64 bits de ancho por 32 bits de alto. Esto representa 2048 bits de información, es decir, 256 bytes de memoria. Cuando un bit de la video memoria estaba a 1, el punto correspondiente en pantalla aparece en blanco (encendido).

El CHIP-8 permite especificar las posiciones de los puntos en pantalla mediante un sistema de coordenadas XY como se muestra en la fig. X Los valores de la coordenada X varían de 00h a 3F (0 a 63 en decimal). Los valores de la coordenada Y varían de 00h a 1Fh (0 a 31 en decimal). Cualquiera de las variables (V0 a VF) pueden ser utilizadas para especificar los valores de las coordenadas X e Y de un punto en pantalla.

La instrucción de dibujo en pantalla (DXYN) permite mostrar no sólo un punto en pantalla, sino un patrón de puntos (dibujo, letra, número). Estos patrones están representados en memoria por una lista de uno hasta un límite de 15 bytes (0 a F). Supongamos que queramos dibujar una nave espacial. Sólo nos queda decirle en qué dirección de memoria se encuentra el patrón de dibujo. Para esto nos servimos del registro especial I. El algoritmo sería

	6200
	6300
	A20A
	D236

es decir, en Ruby

	v2 = 0
	v3 = 0
	I  = 0x20A
	draw_pattern( pos_x = v2, pos_y = v3, memory_address = I, height = 6)

Donde `draw_pattern` sería la función encargada de trasladar la información del patrón de dibujo a la video memoria. La implementación en Ruby sería:

Supongamos que queremos dibujar una personaje en Chip-8. En primer lugar lo dibujaremos en una rejilla. (Esto lo podemos hacer muy bien con Inkscape)!

La instrucción DXYN

La instrucción DXYN compara cada bit del nuevo patrón con lo que esté mostrado en ese mismo punto de la pantalla. Si lo que se muestra es 1 bit en pantalla y el patrón a ese punto también es 1 bit, el resultado del nuevo punto será cero y el registro VF ó V15 tendrá el valor de 01. En realidad, esta operación entre lo que se muestra en pantalla y lo que se tendrá que mostrar funciona bajo la operación lógica EXCLUSIVE OR o XOR. Veamos su tabla de verdad:

Patrón	 Pantalla	 Resultado	 VF
0	 0	 0	 0
1	 0	 1	 0
0	 1	 1	 0
1	 1	 0	 1


Mediante esta simple operación podemos conseguir el efecto básico de animación: un primer frame dibujamos, segundo frame borramos, incrementamos el valor de posición del sprite y, de nuevo, dibujamos. Por otro lado, si este sprite coincide con uno anterior dibujado en pantalla, tenemos también el indicador de colisión al encender el “flag” VF.
