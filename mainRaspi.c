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

#define FD_STDIN 0

int main( int argc, char *argv[] )
{
  int puertoSerial;     // Almacena el descriptor de archivos del puerto serie.
  //char datoRecibido;    // Almacena lo recibido por el puerte serie (byte a byte),
  //char avisaFin[] = "Comunicacion Finalizada";

  //struct termios t_old, t_oldUART, t_new, t_newUART; // Estructuras para atributos del teclado.
  struct termios t_old, t_new; // Estructuras para atributos del teclado.
  //int fdUART;                           // Descriptor de archivo de la UART. 
  //char buf[1]= {'\0'};                  // Almacena el caracter leído por read().


/***********************Apertura del puerto serie************************************/
  puertoSerial = serialOpen("/dev/ttyS0", 9600);
  if( puertoSerial < 0 )
  {
    fprintf(stderr, "Error al abrir \\dev\\ttyS0: %s\n", strerror(errno));
    return 1;
  }

/******************Seteo del modo NO canónico en la ENTRADA ESTANDAR*****************/
  tcgetattr( FD_STDIN, &t_old );        // Lee atributos del teclado.
  t_new = t_old;                        // Guarda los atributos originales.

  t_new.c_lflag &= ~(ECHO | ICANON);  // Anula entrada canónica y eco.
  //t_new.c_cc[VMIN] = 0;               // No espera que ingrese ningún caracter.
  //t_new.c_cc[VTIME] = 0;              // Tampoco espera ningún tiempo.

  tcsetattr( FD_STDIN,TCSANOW,&t_new ); // Setea los valores nuevos de la config.


/****************************Mapeo de pines****************************************/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    fprintf (stdout, "Error al inicializar wirinfPi: %s\n", strerror(errno));
    return 1;
  }

  printf("Llega bien\n");

/*****************************Control de acceso************************************/
  if( controlDeContraseña() == 1)
    printf("\n Bienvenido al sistema\n");
  else{
    printf("\n Contraseña incorrecta\n");
    exit(EXIT_FAILURE);
  }
///**************Inicia lectura y escritura mediante el puerto serie******************/
//  printf(" --------------------------------------\n");
//  printf(" - Com. serie (termina al recibir '!')-\n");
//  printf(" --------------------------------------\n");
//  while( datoRecibido != '!' )
//  {
//    if( serialDataAvail(puertoSerial) )             // Retorna el número de caracteres
//    {                                               //disponible para leer.
//      datoRecibido = serialGetchar( puertoSerial ); // Retorna el siguiente caracter 
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
//      serialPutchar( puertoSerial, avisaFin[i] );
//  }
//  printf("\n");


/*************************Cierre del puerto serie************************************/
  serialClose( puertoSerial );

/****************************Seteo del modo canónico*********************************/
  tcsetattr(FD_STDIN, TCSANOW, &t_old); // Actualiza los atributos del teclado 
                                        //con los valores originales.

  return 0;
}
