#ifndef PROJECTFUNCTIONS_H
#define PROJECTFUNCTIONS_H

#include <stdio.h>
#include <termios.h> //controlContra; seteoModoNOCanonico

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/*-------------------- Seteo de velocidad inicial de secuencias ---------------------------*/
/*------------ Descriptor de entrada estándar y direcciones de registros ------------------*/
#define FD_STDIN 0
#define FD_STDOUT 1
#define ADDRESS 0x48
#define A0 ADDRESS+0
#define A1 ADDRESS+1
#define A2 ADDRESS+2
#define A3 ADDRESS+3


#define FD_STDIN 0
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
void seteoModoNoCanonico( struct termios *t_oldStdIn, struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
int velocidadSecuenciasConPote(  struct termios *t_oldStdIn, struct termios *t_newStdIn );

/*******************************************************************************************/

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void imprimeMenu( void );


#endif
