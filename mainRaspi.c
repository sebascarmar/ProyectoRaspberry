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
  int8_t velSecuencias = 1, modoLocal = 1;
  char opcion = '\0';


/*------------------- Seteo del modo NO canónico en la ENTRADA ESTANDAR -------------------*/
  tcgetattr( FD_STDIN, &t_oldStdIn );          // Lee atributos por defecto del teclado.
  t_newStdIn = t_oldStdIn;                     // Guarda los atributos originales.
  seteoModoNoCanonico( &t_newStdIn );


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

  velSecuencias = velocidadSecuenciasConPote( &t_oldStdIn, &t_newStdIn );

/*-------------------------------- Menú principal -----------------------------------------*/
  while( opcion != 'k' )
  {
    tcsetattr( FD_STDIN,TCSANOW,&t_oldStdIn ); // Setea los valores por defec. de la config.
    imprimeMenu();
    if( modoLocal == 1 )
      opcion = seleccionMenuModoLocal();
    else
    {
      dprintf(FD_STDOUT, "Por favor, ingrese una opción vía UART: ");
      while( serialDataAvail(fdPuertoSerial) == 0 )             // Retorna el número de caracteres
      {                                               //disponible para leer.
        opcion = serialGetchar( fdPuertoSerial ); // Retorna el siguiente caracter 
                                                      //disponible en el dispositivo serial.	
        printf ("%c", opcion);                  // Imprime en pantalla el caracter.
                                                      //el puerto serie.
      }   
    }
    printf("\n");

    switch( opcion )
    {
      case 'a': // Modo local/remoto.
        printf("-----------------------------------------------------------------------------\n");
        printf("Seleccione el modo de control:\n"
               "\t 1) Local\n"
               "\t 2) Remoto\n"
               "Modo: ");

        scanf("%hhd", &modoLocal);
        printf("\n");
        break;
      
      case 'b': // Velocidad con pote.
        printf("-----------------------------------------------------------------------------\n");
        printf("Seleccione la velocidad de las secuencias con el potenciómetro del ADC\n");
      
        seteoModoNoCanonico( &t_newStdIn );
        seteoModoNoBloqueante( &t_newStdIn );
        velSecuencias = velocidadSecuenciasConPote( &t_oldStdIn, &t_newStdIn );
        break;
      
      case 'c': // Sec. auto fantástico.
        break;
      
      case 'd': // Sec. choque.
        break;
      
      case 'e': // Sec. apilada.
        break;
      
      case 'f': // Sec. carrera.
        break;
      
      case 'g':
        break;
      
      case 'h':
        break;
      
      case 'i':
        break;
        
      case 'j':
        break;
      
      case 'k': // Salir del programa.
        break;
      
      default:
        break;
    } // Fin del switch case.
  }





/*---------------------------- Cierre del puerto serie ------------------------------------*/
  serialClose( fdPuertoSerial );

/*------------------------------ Seteo del modo canónico ----------------------------------*/
  tcsetattr(FD_STDIN, TCSANOW, &t_oldStdIn); // Actualiza los atributos del teclado 
                                             //con los valores originales.

  return 0;
}
