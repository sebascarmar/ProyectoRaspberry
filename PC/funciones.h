#ifndef FUNCIONES_H
#define FUNCIONES_H

/*------------------------------- Archivos de cabecera ------------------------------------*/
#include <stdio.h> // printf, dprintf
#include <stdlib.h> //exit
#include <unistd.h> //read, usleep
#include <termios.h> // struct termios, tcgetattr, tcsetattr, tcflush

#include <stdbool.h>

/*------------------ Descriptor de archivo de los archivos estándar -----------------------*/
#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2


/*#########################################################################################*/
/*##                                     PARÁMETROS TTY                                  ##*/
/*#########################################################################################*/

/* ACCIÓN: setea el modo NO canónico en la tty apuntada por ttyNew.
 * PARÁMETROS: un entero con el descriptor de archivos de la tty en cuestión
 *             un puntero a la estructura termios de la terminal a configurar
 * RETORNO: ninguno.
 */
void seteoModoNoCanonicoNoBloqueante( int fd, struct termios *ttyNew );

/*******************************************************************************************/

/* ACCIÓN: setea la trama 8N1 y los baudios de la tty apuntada por ttyNew.
 * PARÁMETROS: un entero con el descriptor de archivos de la tty en cuestión
 *             un puntero a la estructura termios de la terminal a configurar
 * RETORNO: ninguno.
 */
void seteoTramaYBaudRate( int fd, struct termios *ttyNew, speed_t baudRate );


/*#########################################################################################*/
/*##                           FUNCIONES PARA EL MENÚ                                    ##*/
/*#########################################################################################*/

/* ACCIÓN: imprime las opciones del menú principal en la salida estándar.
 * PARÁMETROS: ninguno.
 * RETORNO: ninguno.
 */
void imprimeMenu( void );

/*******************************************************************************************/

/* ACCIÓN: selecciona las opciones ingresadas por el teclado, las envía por puerto
 *        serie y las imprime en pantalla, haciendo un control de opciones válidas.
 * PARÁMETROS: un entero con el descriptor de archivos del puerto serie.
 * RETORNO: ninguno.
 */
char seleccionMenuModoRemoto( int fdUART );

/*******************************************************************************************/

/* ACCIÓN: da aviso que se eligió el modo local, y solo permite avanzar si se elige el
 *        modo remoto nuevamente.
 * PARÁMETROS: un entero con el descriptor de archivos del puerto serie.
 * RETORNO: ninguno.
 */
char seleccionMenuModoLocal( void );


/*#########################################################################################*/
/*##                           ELECCIÓN DE MODO LOCAL O REMOTO                           ##*/
/*#########################################################################################*/

/* ACCIÓN: seleccion el modo local o remoto.
 * PARÁMETROS: un entero con el descriptor de archivos del puerto serie
 *             un bool indicando si se está en modo local o remoto al momento del llamado
 * RETORNO: un bool: verdadero si es modo local, falso si es modo remoto.
 */
bool seleccionModo( int fdUART, bool modoLocal );


/*#########################################################################################*/
/*##                                SECUENCIAS                                           ##*/
/*#########################################################################################*/

/* ACCIÓN: detecta lo ingresado por teclado y lo envía por puerto serie, durante la secuencia
 * PARÁMETROS: un entero con el descriptor de archivos del puerto serie.
 * RETORNO: ninguno.
 */
void secuencia( int fdUART );


#endif



