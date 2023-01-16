#ifndef PROJECTFUNCTIONS_H
#define PROJECTFUNCTIONS_H

#include <stdio.h>
#include <termios.h> //controlContra; seteoModoNOCanonico

#define FD_STDIN 0
#define LENGTH_PSSW 5

/* FUNCION:  controlDeContraseña
 * ACCION: realiza un control de acceso al programa.
 * PARAMETROS: ninguno.
 * RETORNO: retorna un entero, 1 si se logra acceder y 0 si no.
 */
int controlDeContraseña( void );

/*--------------------------------------------------------------------------------------------------*/ 

/* FUNCION: 
 * ACCION:
 * PARAMETROS:
 * RETORNO:
 */
void seteoModoNoCanonico( struct termios *t_oldStdIn, struct termios *t_newStdIn );




#endif
