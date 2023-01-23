#ifndef PROJECTFUNCTIONS_H
#define PROJECTFUNCTIONS_H

#include <stdio.h> // xprintf
#include <stdlib.h> //exit, int8_t
#include <unistd.h> //read, usleep
#include <termios.h> //controlContra; seteoModoNOCanonico, etc ; lecturaTeclado [local]

#include <wiringPiI2C.h> //comunicación con el ADC
#include <wiringSerial.h> //lecturaTeclado [remoto]

#include <stdbool.h>

#include "easyPio.h"

/*-------------------- Seteo de velocidad inicial de secuencias ---------------------------*/
/*------------ Descriptor de entrada estándar y direcciones de registros ------------------*/
#define FD_STDIN 0
#define FD_STDOUT 1

#define ADDRESS 0x48
#define A0 ADDRESS+0
#define A1 ADDRESS+1
#define A2 ADDRESS+2
#define A3 ADDRESS+3

#define LENGTH_PSSW 5


extern void retardo( int a ); // Función escrita en ensamblador de ARM.



/* ACCION: realiza un control de acceso al programa.
 * PARAMETROS: ninguno.
 * RETORNO: retorna un entero, 1 si se logra acceder y 0 si no.
 */
int controlDeContraseña( void );

/*******************************************************************************************/

/* ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoNoCanonico( struct termios *ttyNewStdIn );

/*******************************************************************************************/

/* ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoNoBloqueante( struct termios *ttyNewStdIn );

/*******************************************************************************************/

/* ACCION: setea la velocidad entre secuencias con el potencióemtro del ADC.
 * PARAMETROS: dos punteros a estructuras termios.
 * RETORNO: un entero con el valor de velocidad (de mín=1 a máx=10).
 */
int velocidadSecuenciasConPote( void );

/*******************************************************************************************/

/* ACCION: imprime las opciones del menú principal en la stdout.
 * PARAMETROS: ninguno.
 * RETORNO: ninguno.
 */
void imprimeMenu( void );

/*******************************************************************************************/

/* ACCION: seleccion el modo local o remoto, estando en modo local.
 * PARAMETROS: ninguno.
 * RETORNO: un bool: verdadero si es modo local, falso si es modo remoto.
 */
bool seleccionModoEnModoLocal( void);

/*******************************************************************************************/

/* ACCION: en base a la velocidad de la secuencia elegida, setea el valor del delay.
 * PARAMETROS: un entero con el valor de la velocidad de la secuencia deseada.
 * RETORNO: un entero con el valor de delay.
 */
int valorDeRetardo( int velSecuencia );

/*******************************************************************************************/

/* ACCION: setea la velocdad entre secuencis con las flechas up/down del teclado (Loc o Rem)
 * PARAMETROS: un puntero a un entero de 8bits, un arreglo de char.
 * RETORNO: ninguno.
 */
void velocidadSecuenciasConTeclado( int8_t *velSecuencias, char *buf );

/*******************************************************************************************/

/* ACCION: imprime durante la ejecución el valor de velocidad, para ver los cambios en vivo.
 * PARAMETROS: un enetero con la velocidad de las secuencias ya seteada.
 * RETORNO: ninguno.
 */
void imprimeVelocidadDurante( int velSecuencias );

/*******************************************************************************************/

/* ACCION: durante la ejecución de las secuencias, lee el teclado local o remoto (mediante
 * el puerto serie).
 * PARAMETROS: un char con el modo de manejo
 *             un entero con el descr. de archivos del puerto serial,
 *             un un arreglo a char para almacenar lo leído
 * RETORNO: ninguno.
 */
void lecturaTeclado( bool modoLocal, int fdPuertoSerial, char *buf );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia El Auto Fantástico en los leds
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secAutoFantastico( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia El Choque en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secChoque( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia la Apilada en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secApilada( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia La Carrera en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secCarrera( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia El Vúmetro en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secVumetro( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia La Gran Moisés en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secJuntosPorParidad( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia La Gran Moisés en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secGranMoises( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* ACCION: ejecuta la secuencia El Parpadeo en los leds.
 * PARAMETROS: un arreglo a enteros con los pines de los leds
 *             un puntero a entero de 8 bit con la dirección del valor de velocidad de secuen.
 *             un char con el modo de manejo ('1' local, '0' remoto)
 *             un entero con el valor del puerto serial
 * RETORNO: ninguno.
 */
void secParpadeo( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial );



#endif
