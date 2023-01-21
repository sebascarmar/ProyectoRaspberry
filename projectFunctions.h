#ifndef PROJECTFUNCTIONS_H
#define PROJECTFUNCTIONS_H

#include <stdio.h> // xprintf
#include <stdlib.h> //exit, int8_t
#include <unistd.h> //read, usleep
#include <termios.h> //controlContra; seteoModoNOCanonico, etc ; lecturaTeclado [local]

#include <wiringPi.h>
#include <wiringPiI2C.h> //comunicación con el ADC
#include <wiringSerial.h> //lecturaTeclado [remoto]

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


extern void retardo(unsigned long int a);



/* FUNCION:  controlDeContraseña
 * ACCION: realiza un control de acceso al programa.
 * PARAMETROS: ninguno.
 * RETORNO: retorna un entero, 1 si se logra acceder y 0 si no.
 */
int controlDeContraseña( void );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoNoCanonico( struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoNoBloqueante( struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoCanonico( struct termios *t_oldStdIn, struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoBloqueante( struct termios *t_oldStdIn, struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
int velocidadSecuenciasConPote( struct termios *t_oldStdIn, struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void imprimeMenu( void );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
int valorDeRetardo( int velSecuencia );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void velocidadSecuenciasConTeclado( int8_t *velSecuencias, char *buf );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void imprimeVelocidadDurante( int velSecuencias );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void lecturaTeclado( char modoLocal, int fdPuertoSerial, char *buf );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secAutoFantastico( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secChoque( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secApilada( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secCarrera( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secVumetro( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secJuntosPorParidad( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secGranMoises( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void secParpadeo( int *leds, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial );



#endif
