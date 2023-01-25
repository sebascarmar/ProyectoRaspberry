#ifndef FUNCIONES_H
#define FUNCIONES_H

/*------------------------------- Archivos de cabecera ------------------------------------*/
#include <stdio.h> // printf, dprintf
#include <stdlib.h> //exit, int8_t
#include <unistd.h> //read, usleep
#include <termios.h> // struct termios, tcgetattr, tcsetattr, tcflush

#include <wiringPiI2C.h> //comunicación con el ADC
#include <wiringSerial.h> //lectura puerto serial

#include <stdbool.h>

#include "easyPio.h"

/*----------------- Descriptor de entrada estándar y salida estándar ----------------------*/
#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

/*-------------- Direcciones de registros  usados para comunicación I2C -------------------*/
#define ADDRESS 0x48
#define A0 ADDRESS+0
#define A1 ADDRESS+1
#define A2 ADDRESS+2
#define A3 ADDRESS+3

/*---------------------------- Tamaño de la contraseña ------------------------------------*/
#define LENGTH_PSSW 5



/*#########################################################################################*/
/*##                         CONFIGURACIÓN DE LA ENTRADA ESTÁNDAR                        ##*/
/*#########################################################################################*/

/* ACCIÓN: setea el modo NO canónico en la entrada estándar.
 * PARÁMETROS: un puntero a la estructura termios de la terminal a configurar.
 * RETORNO: ninguno.
 */
void seteoModoNoCanonico( struct termios *ttyNewStdIn );

/*******************************************************************************************/

/* ACCIÓN: setea el modo NO bloqueante en la entrada estándar.
 * PARÁMETROS: un puntero a la estructura termios de la terminal a configurar.
 * RETORNO: ninguno.
 */
void seteoModoNoBloqueante( struct termios *ttyNewStdIn );


/*#########################################################################################*/
/*##                                  CONTROL DE INGRESO                                 ##*/
/*#########################################################################################*/

/* ACCIÓN: realiza un control de acceso al programa.
 * PARÁMETROS: ninguno.
 * RETORNO: retorna un entero, 1 si se logra acceder y 0 si no.
 */
int controlDeContraseña( void );


/*#########################################################################################*/
/*##                               VELOCIDAD DE SECUENCIAS                               ##*/
/*#########################################################################################*/

/* ACCIÓN: setea la velocidad entre secuencias con el potencióemtro del ADC.
 * PARÁMETROS: ninguno.
 * RETORNO: un entero con el valor de velocidad (desde mín=1 hasta máx=10).
 */
int velocidadSecuenciasConPote( void );

/*******************************************************************************************/

/* ACCIÓN: setea la velocidad durante la secuencia  en ejecución, con las flechas up/down del
 *       teclado local (entrada estándar) o remoto (mediante puerto serial).
 * PARÁMETROS: un puntero a un entero de 8bits, un arreglo de char.
 * RETORNO: ninguno.
 */
void velocidadSecuenciasConTeclado( int8_t *velSecuencias, char *buf );

/*******************************************************************************************/

/* ACCIÓN: realiza un retardo restándole 1 a un número hasta llegar a cero.
 * PARÁMETROS: un entero.
 * RETORNO: ninguno.
 */
extern void retardo( int a ); // Función escrita en ensamblador de ARM.

/*******************************************************************************************/

/* ACCIÓN: en base a la velocidad de la secuencia elegida, setea el valor del delay.
 * PARÁMETROS: un entero con el valor de la velocidad de la secuencia deseada.
 * RETORNO: un entero con el valor de delay.
 */
int valorDeRetardo( int velSecuencia );


/*#########################################################################################*/
/*##                           FUNCIONES PARA EL MENÚ                                    ##*/
/*#########################################################################################*/

/* ACCIÓN: imprime las opciones del menú principal en la stdout.
 * PARÁMETROS: ninguno.
 * RETORNO: ninguno.
 */
void imprimeMenu( void );

/*******************************************************************************************/

/* ACCIÓN: permite elegir una opción del menú, controlando las entradas no válidas. Esta
 *        tarea la realiza estando en modo local, es decir, lee el TECLADO.
 * PARÁMETROS: ninguno.
 * RETORNO: un char con una opción válida del menú.
 */
extern char seleccionMenuModoLocal( void ); // Función escrita en ensamblador de ARM.

/*******************************************************************************************/

/* ACCIÓN: permite elegir una opción del menú, controlando las entradas no válidas. Esta
 *        tarea la realiza estando en modo remoto, es decir, lee el PUERTO SERIAL.
 * PARÁMETROS: un entero con el descriptor de archivo del puerto serial.
 * RETORNO: un char con una opción válida del menú.
 */
char seleccionMenuModoRemoto( int fdPuertoSerial );


/*#########################################################################################*/
/*##                           ELECCIÓN DE MODO LOCAL O REMOTO                           ##*/
/*#########################################################################################*/

/* ACCIÓN: seleccion el modo local o remoto, estando en modo local.
 * PARÁMETROS: ninguno.
 * RETORNO: un bool: verdadero si es modo local, falso si es modo remoto.
 */
bool seleccionModoEnModoLocal( void );

/*******************************************************************************************/

/* ACCIÓN: seleccion el modo local o remoto, estando en modo remoto.
 * PARÁMETROS: un entero con el descriptor de archivos del puerto serial.
 * RETORNO: un bool: verdadero si es modo local, falso si es modo remoto.
 */
bool seleccionModoEnModoRemoto( int fdPuertoSerial );


/*#########################################################################################*/
/*##                   MANEJO DE STDIN Y STDOUT DURANTE SECUENCIAS                       ##*/
/*#########################################################################################*/

/* ACCIÓN: imprime durante la ejecución el valor de velocidad, para ver los cambios en vivo.
 * PARÁMETROS: un enetero con la velocidad de las secuencias ya seteada.
 * RETORNO: ninguno.
 */
void imprimeVelocidadDurante( int velSecuencias );

/*******************************************************************************************/

/* ACCIÓN: durante la ejecución de las secuencias, lee el teclado local o remoto (mediante
 * el puerto serie).
 * PARÁMETROS: un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el descr. de archivos del puerto serial,
 *             un un arreglo a char para almacenar lo leído
 * RETORNO: ninguno.
 */
void lecturaTeclado( bool modoLocal, int fdPuertoSerial, char *buf );


/*#########################################################################################*/
/*##                          SECUENCIAS DE LUCES                                        ##*/
/*#########################################################################################*/

/* ACCIÓN: ejecuta la secuencia El Auto Fantástico en los leds
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secAutoFantastico( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia El Choque en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secChoque( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia la Apilada en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secApilada( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia La Carrera en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secCarrera( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia El Vúmetro en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secVumetro( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia La Gran Moisés en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secJuntosPorParidad( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia La Gran Moisés en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secGranMoises( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCIÓN: ejecuta la secuencia El Parpadeo en los leds.
 * PARÁMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un bool con el modo de manejo (true: local, false: remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secParpadeo( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );



#endif
