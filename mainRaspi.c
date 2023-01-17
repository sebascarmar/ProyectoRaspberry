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


int main( int argc, char *argv[] )
{
  int fdPuertoSerial;     // Almacena el descriptor de archivos del puerto serie.
  struct termios t_oldStdIn, t_newStdIn; // Estructuras para atributos del teclado.
  int leds[8] = {23, 24, 25, 12, 16, 20, 21, 26}; // Arreglo que contiene los leds.


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

/****************************Mapeo de pines****************************************/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    fprintf (stdout, "Error al inicializar wiringPi: %s\n", strerror(errno));
    tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }


///**************Inicia lectura y escritura mediante el puerto serie******************/
//  printf(" --------------------------------------\n");
//  printf(" - Com. serie (termina al recibir '!')-\n");
//  printf(" --------------------------------------\n");
//  while( datoRecibido != '!' )
//  {
//    if( serialDataAvail(fdPuertoSerial) )             // Retorna el número de caracteres
//    {                                               //disponible para leer.
//      datoRecibido = serialGetchar( fdPuertoSerial ); // Retorna el siguiente caracter 
//                                                    //disponible en el dispositivo serial.	
//      printf ("%c", datoRecibido);                  // Imprime en pantalla el caracter.
//                                                    //el puerto serie.
//    }
//  }
//  delay(1); // Sin esto la PC no lee el 1er char de la cadena a enviar.
//
///************Envía cadena de aviso de finalización por puerto serie*****************/
//  for( int i = 0 ; i <= strlen(avisaFin) ; i++ )
//  {
//      serialPutchar( fdPuertoSerial, avisaFin[i] );
//  }
//  printf("\n");


/*************************Cierre del puerto serie************************************/
  serialClose( fdPuertoSerial );

/****************************Seteo del modo canónico*********************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                        //con los valores originales.

  return 0;
}
