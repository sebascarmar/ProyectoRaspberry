#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include <stdbool.h>

#include "funciones.h"



int main( int argc, char *argv[] )
{
  struct termios ttyOldStdIn, ttyNewStdIn; // Estructuras para atributos del teclado.
  int fdPuertoSerial;     // Almacena el descriptor de archivos del puerto serie.
  int leds[8] = {23, 24, 25, 12, 16, 20, 21, 26}; // Arreglo que contiene los leds.
  int8_t velSecuencias = 1; // Almacena la velocidad del 1 al 10.
  char opcion = '\0'; // Almacena la opción que se elige del menú principal.
  bool modoLocal = true;    // Establece si el control  del programa se hace local o remoto.


/*------------------- Seteo del modo NO canónico en la ENTRADA ESTANDAR -------------------*/
  tcgetattr( FD_STDIN, &ttyOldStdIn );          // Lee atributos por defecto del teclado.
  ttyNewStdIn = ttyOldStdIn;                     // Guarda los atributos originales.
  seteoModoNoCanonico( &ttyNewStdIn );


/*-------------------------------- Control de acceso --------------------------------------*/
  printf("-----------------------------------------------------------------------\n"
         "Para continuar, por favor ingrese su contraseña\n");
  if( controlDeContraseña( ) == 1 )
  {
    printf("\n\n\t\t\t ¡BIENVENIDO AL SISTEMA!\n");
  } else
  {
    printf("\n\t\t\t NO SE HA PODIDO INGRESAR AL SISTEMA\n");
    tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }


/*------------------ Seteo del modo NO bloqueante en la ENTRADA ESTANDAR ------------------*/
  seteoModoNoBloqueante( &ttyNewStdIn );


/*---------- Mapeo y configuración de pines según EasyPIO.h (manejo de GPIO ) -------------*/
  pioInit( );
  
  for(int i=0 ; i < 8 ; i++)
    pinModeEP( leds[i], OUTPUT );


/*----------------------------- Apertura del puerto serie ---------------------------------*/
  fdPuertoSerial = serialOpen("/dev/ttyS0", 9600);
  if( fdPuertoSerial < 0 )
  {
    printf("Error al abrir \\dev\\ttyS0\n");
    tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }


/*---------------------- Mapeo de pines según WiringPi (UART) -----------------------------*/
  if ( wiringPiSetup( ) == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
  {
    printf("Error al inicializar wiringPi.\n");
    tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }


/*-------------------- Seteo de velocidad inicial de secuencias ---------------------------*/
  printf("-----------------------------------------------------------------------\n"
         "Seleccione la velocidad de las secuencias con el potenciómetro del ADC\n"
         "(mín=1, máx=10)\n");

  velSecuencias = velocidadSecuenciasConPote( );


/*-------------------------------- Menú principal -----------------------------------------*/
  while( opcion != 'k' )
  {
    imprimeMenu( );
    if( modoLocal == true )
      opcion = seleccionMenuModoLocal( );
    else
    {
      opcion = seleccionMenuModoRemoto( fdPuertoSerial );
    }
    dprintf(FD_STDOUT, "\n\n");

    switch( opcion )
    {
      case 'a': // Modo remoto/local.
        dprintf(FD_STDOUT, "--------------------------------------------------------------"
                           "---------\n"
                           "Modo de control:\n"
                           "\t 1) Local\n"
                           "\t 2) Remoto\n\n"
                           "Por favor, elija el modo: ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        if( modoLocal == true ) // Selección de modo en modo local.
        {
          modoLocal = seleccionModoEnModoLocal( );
          
        }else                   // Selección de modo en modo remoto.
        {
            modoLocal = seleccionModoEnModoRemoto( fdPuertoSerial );
        }
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'b': // Velocidad con pote.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "Seleccione la velocidad de las secuencias con el potenciómetro del ADC\n"
               "(mín=1, máx=10)\n");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        velSecuencias = velocidadSecuenciasConPote( );
        break;
      
      case 'c': // Secuencia El Auto Fantástico.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Auto Fantástico\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secAutoFantastico( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'd': // Secuencia El Choque.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Choque\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secChoque( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'e': // Secuencia La Apilada.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"La Apilada\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secApilada( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'f': // Secuencia La Carrera.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"La Carrera\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secCarrera( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'g': // Secuencia El Vúmetro.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Vúmetro\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secVumetro( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'h': // Secuencia Juntos Por Paridad.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"Juntos Por Paridad\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secJuntosPorParidad( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'i': // Secuencia La Gran Moisés.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"La Gran Moisés\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secGranMoises( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
        
      case 'j': // Secuencia El Parpadeo.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Parpadeo\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
        retardo( 1000000 );  // Retardo que permite que se imprima el mensaje siempre.
      
        secParpadeo( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      default:
        break;
    } // Fin del switch case.
  }



/*---------------------------- Cierre del puerto serie ------------------------------------*/
  serialClose( fdPuertoSerial );

/*------------------------------ Seteo del modo canónico ----------------------------------*/
  tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
                                              //con los valores originales.

  return 0;
}


