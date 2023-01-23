#ifndef EASY_PIO_H
#define EASY_PIO_H

/*#########################################################################################*/
/*##                            Inclusión de headers                                     ##*/
/*#########################################################################################*/
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // Posee las funciones para manip. archvivos open()/close().
#include <stdlib.h> // Posee la función exit().

/*#########################################################################################*/
/*##                                  Constantes                                         ##*/
/*#########################################################################################*/

// Tipo de función GPIO FSEL (6 registros, 3 bits por cada GPIO).
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

/*#########################################################################################*/
/*##                                 Mapeo de memoria                                     #*/
/*#########################################################################################*/
#define BCM2835_PERI_BASE   0x3F000000 // Dirección base de todos los periféricos.
#define GPIO_BASE           (BCM2835_PERI_BASE + 0x200000) // Dir. base del GPIO.
#define BLOCK_SIZE          (4*1024)


/*#########################################################################################*/
/*##                                   Registros GPIO                                    ##*/
/*#########################################################################################*/
#define GPFSEL              ((volatile unsigned int *)(gpio + 0) )
#define GPSET               ((volatile unsigned int *)(gpio + 7) )
#define GPCLR               ((volatile unsigned int *)(gpio + 10) )
#define GPLEV               ((volatile unsigned int *)(gpio + 13) )

/*#########################################################################################*/
/*##                                Funciones generales                                  ##*/
/*#########################################################################################*/

/* ACCIÓN: realiza el mapeo de todos los pines.
 * PARAMETROS: ninguno.
 * RETORNO: ninguno.
 */
void pioInit( void );


/*#########################################################################################*/
/*##                                Funciones GPIO                                       ##*/
/*#########################################################################################*/

/* ACCIÓN: inicializa los pines como propósito general o especial.
 * PARAMETROS: un entero que representa el pin que se desea.
 *             un entero que representa la función que tendrá ese pin.
 * RETORNO: ninguno.
 */
void pinModeEP( int pin, int function ); // Los registros GPFSEL definen la fun. de cada pin.

/*******************************************************************************************/

/* ACCIÓN: escribe en los pines el estado ALTO o BAJO.
 * PARAMETROS: un entero que representa el pin que se desea.
 *             un entero que con el valor lógico a escribir.
 * RETORNO: ninguno.
 */
void digitalWriteEP( int pin, int val );

/*******************************************************************************************/

/* ACCIÓN: lee el estado lógico de un pin.
 * PARAMETROS: un entero que representa el pin que se desea.
 * RETORNO: un entero con el estado del pin (1 o 0)
 */
int digitalReadEP( int pin );


#endif
