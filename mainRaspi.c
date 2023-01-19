#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "projectFunctions.h"
#include "EasyPIO.h"

#include <stdint.h>

extern char seleccionMenuModoLocal( void );

int main( int argc, char *argv[] )
{
  int fdPuertoSerial;     // Almacena el descriptor de archivos del puerto serie.
  struct termios t_oldStdIn, t_newStdIn; // Estructuras para atributos del teclado.
  int leds[8] = {23, 24, 25, 12, 16, 20, 21, 26}; // Arreglo que contiene los leds.
  int8_t velSecuencias = 1;
  char opcion = '\0';


/*------------------- Seteo del modo NO canónico en la ENTRADA ESTANDAR -------------------*/
  seteoModoNoCanonico( &t_oldStdIn, &t_newStdIn );


/*-------------------------------- Control de acceso --------------------------------------*/
  printf("-----------------------------------------------------------------------------\n");
  printf("Para continuar, por favor ingrese su contraseña\n");
  if( controlDeContraseña() == 1)
  {
    printf("\n\n\t\t\t ¡BIENVENIDO AL SISTEMA!\n");
  } else
  {
    printf("\n\t\t\t NO SE HA PODIDO INGRESAR AL SISTEMA\n");
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/*---------- Mapeo y configuración de pines según EasyPIO.h (manejo de GPIO ) -------------*/
  pioInit();
  
  for(int i=0 ; i < 8 ; i++)
    pinModeEP( leds[i], OUTPUT );

/*----------------------------- Apertura del puerto serie ---------------------------------*/
  fdPuertoSerial = serialOpen("/dev/ttyS0", 9600);
  if( fdPuertoSerial < 0 )
  {
    fprintf(stderr, "Error al abrir \\dev\\ttyS0: %s\n", strerror(errno));
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/*---------------------- Mapeo de pines según WiringPi (UART) -----------------------------*/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    fprintf (stdout, "Error al inicializar wiringPi: %s\n", strerror(errno));
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/*-------------------- Seteo de velocidad inicial de secuencias ---------------------------*/
  printf("-----------------------------------------------------------------------------\n");
  printf("Seleccione la velocidad de las secuencias con el potenciómetro del ADC\n");

  velSecuencias = velocidadSecuenciasConPote(  &t_oldStdIn, &t_newStdIn );



/*-------------------------------- Menú principal -----------------------------------------*/
  tcsetattr( FD_STDIN,TCSANOW,&t_oldStdIn ); // Setea los valores por defec. de la config.
  imprimeMenu();
  opcion = seleccionMenuModoLocal();
  printf("La opción ingresada fue: %c\n", opcion);






/*---------------------------- Cierre del puerto serie ------------------------------------*/
  serialClose( fdPuertoSerial );

/*------------------------------ Seteo del modo canónico ----------------------------------*/
  tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                             //con los valores originales.

  return 0;
}
