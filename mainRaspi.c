#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "projectFunctions.h"
#include "EasyPIO.h"

#include <stdint.h>


int main( int argc, char *argv[] )
{
  int fdPuertoSerial;     // Almacena el descriptor de archivos del puerto serie.
  struct termios t_oldStdIn, t_newStdIn; // Estructuras para atributos del teclado.
  int leds[8] = {23, 24, 25, 12, 16, 20, 21, 26}; // Arreglo que contiene los leds.
  int8_t velSecuencias = 1;


/***************** Seteo del modo NO canónico en la ENTRADA ESTANDAR ****************/
  seteoModoNoCanonico( &t_oldStdIn, &t_newStdIn );


/***************************** Control de acceso ************************************/
  if( controlDeContraseña() == 1)
    printf("\n\n\t\t\t BIENVENIDO AL SISTEMA\n");
  else
  {
    printf("\n\n\t\t\t NO SE HA PODIDO INGRESAR AL SISTEMA\n");
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/****** Mapeo y configuración de pines según EasyPIO.h (manejo de GPIO ) ************/
  pioInit();
  
  for(int i=0 ; i < 8 ; i++)
    pinModeEP( leds[i], OUTPUT );

/***********************Apertura del puerto serie************************************/
  fdPuertoSerial = serialOpen("/dev/ttyS0", 9600);
  if( fdPuertoSerial < 0 )
  {
    fprintf(stderr, "Error al abrir \\dev\\ttyS0: %s\n", strerror(errno));
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/**************************** Mapeo de pines ****************************************/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    fprintf (stdout, "Error al inicializar wiringPi: %s\n", strerror(errno));
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/***************** Seteo de velocidad inicial de secuencias *************************/
  // Modo no bloqueante de la entrada estandar.
  t_newStdIn.c_cc[VMIN] = 0;    // No espera a recibir ningún caracter.
  t_newStdIn.c_cc[VTIME] = 0;   // No espera tiempo alguno a recibir ningún caracter
  tcsetattr( FD_STDIN,TCSANOW,&t_newStdIn ); // Setea los valores nuevos de la config.

  velSecuencias = velocidadInicialSecuencias( );

  // Modo por defecto de la entrada estandar.
  t_newStdIn.c_cc[VMIN] = t_oldStdIn.c_cc[VMIN];   // Setea valor por defecto.
  t_newStdIn.c_cc[VTIME] = t_oldStdIn.c_cc[VTIME]; // Setea valor por defecto.
  tcsetattr( FD_STDIN,TCSANOW,&t_newStdIn ); // Setea los valores nuevos de la config.

    printf("\n velocidad: %d \n", velSecuencias);

/****************************** Menú principal ***************************************/






/*************************Cierre del puerto serie************************************/
  serialClose( fdPuertoSerial );

/****************************Seteo del modo canónico*********************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                        //con los valores originales.

  return 0;
}
