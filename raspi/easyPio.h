#ifndef EASY_PIO_H
#define EASY_PIO_H

/*############################################################################*/
/*##                      Inclusión de headers                              ##*/
/*############################################################################*/
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> // Posee las funciones para manip. archvivos open()/close().
#include <stdlib.h> // Posee la función exit().

/*############################################################################*/
/*##                         Constantes                                     ##*/
/*############################################################################*/

// Tipo de función GPIO FSEL (6 registros, 3 bits por cada GPIO).
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

/*###############################################################################*/
/*##                           Mapeo de memoria                                 #*/
/*###############################################################################*/
#define BCM2835_PERI_BASE   0x3F000000 // Dirección base de todos los periféricos.
#define GPIO_BASE           (BCM2835_PERI_BASE + 0x200000) // Dir. base del GPIO.
#define BLOCK_SIZE          (4*1024)


/*###############################################################################*/
/*##                             Registros GPIO                                ##*/
/*###############################################################################*/
#define GPFSEL              ((volatile unsigned int *)(gpio + 0) )
#define GPSET               ((volatile unsigned int *)(gpio + 7) )
#define GPCLR               ((volatile unsigned int *)(gpio + 10) )
#define GPLEV               ((volatile unsigned int *)(gpio + 13) )

/*###############################################################################*/
/*##                          Funciones generales                              ##*/
/*###############################################################################*/
void pioInit( void );


/*###############################################################################*/
/*##                         Funcionetes GPIO                                  ##*/
/*###############################################################################*/

void pinModeEP( int pin, int function ); // Los registros GPFSEL determinan la fun. de cada pin.
/*-------------------------------------------------------------------------------*/ 
void digitalWriteEP( int pin, int val );
/*-------------------------------------------------------------------------------*/ 
int digitalReadEP( int pin );

#endif
