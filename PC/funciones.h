#ifndef FUNCIONES_H
#define FUNCIONES_H

/*------------------------------- Archivos de cabecera ------------------------------------*/
#include <stdio.h> // printf, dprintf
#include <stdlib.h> //exit
#include <unistd.h> //read, usleep
#include <termios.h> // struct termios, tcgetattr, tcsetattr, tcflush

/*----------------- Descriptor de entrada estándar y salida estándar ----------------------*/
#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2


/*#########################################################################################*/
/*##                                     PARÁMETROS TTY                                  ##*/
/*#########################################################################################*/

/* ACCIÓN: setea el modo NO canónico en la entrada estándar.
 * PARÁMETROS: un puntero a la estructura termios de la terminal a configurar.
 * RETORNO: ninguno.
 */
void seteoModoNoCanonicoNoBloqueante( int fd, struct termios *ttyNew );

/*******************************************************************************************/

/* ACCIÓN: setea el modo NO canónico en la entrada estándar.
 * PARÁMETROS: un puntero a la estructura termios de la terminal a configurar.
 * RETORNO: ninguno.
 */
void seteoTramaYBaudRate( int fd, struct termios *ttyNew, speed_t baudRate );


/*#########################################################################################*/
/*##                           FUNCIONES PARA EL MENÚ                                    ##*/
/*#########################################################################################*/

/* ACCIÓN: imprime las opciones del menú principal en la stdout.
 * PARÁMETROS: ninguno.
 * RETORNO: ninguno.
 */
void imprimeMenu( void );

/*******************************************************************************************/

/* ACCIÓN: imprime las opciones del menú principal en la stdout.
 * PARÁMETROS: ninguno.
 * RETORNO: ninguno.
 */
char seleccionMenu( int fdUART );


/*#########################################################################################*/
/*##                           ELECCIÓN DE MODO LOCAL O REMOTO                           ##*/
/*#########################################################################################*/

/* ACCIÓN: seleccion el modo local o remoto, estando en modo local.
 * PARÁMETROS: ninguno.
 * RETORNO: un bool: verdadero si es modo local, falso si es modo remoto.
 */
void seleccionModo( int fdUART );


/*#########################################################################################*/
/*##                                SECUENCIAS                                           ##*/
/*#########################################################################################*/

/* ACCIÓN: seleccion el modo local o remoto, estando en modo local.
 * PARÁMETROS: ninguno.
 * RETORNO: un bool: verdadero si es modo local, falso si es modo remoto.
 */
void secuencia( int fdUART );


#endif



