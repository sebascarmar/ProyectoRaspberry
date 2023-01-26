#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include "funciones.h"


int main( int argc, char *argv[] )
{  
  struct termios ttyOldStdIn, ttyNewStdIn; // Estructuras para atributos del teclado.
  struct termios ttyOldUART, ttyNewUART;   // Estructuras para atributos de la UART.
  int fdUART;                              // Descriptor de archivo de la UART. 
  char opcion = '\0';                      // Almacena el caracter ingresado.
  bool modoLocal = false;

/*------------ Seteo del modo NO canónico NO bloqueante en la ENTRADA ESTANDAR ------------*/
  tcgetattr( FD_STDIN, &ttyOldStdIn );              // Lee atributos del teclado.
  ttyNewStdIn = ttyOldStdIn;                        // Guarda los atributos originales.

  seteoModoNoCanonicoNoBloqueante( FD_STDIN, &ttyNewStdIn );


/*-------------------------- Apertura del puerto serie (UART) -----------------------------*/
  
  if( argc != 2 ) // Corrobora que  el main se ejcute con argumentos.
  {
    dprintf(FD_STDERR, "ERROR EN MODO DE EJECUCIÓN: %s <UART> \n", argv[0]);
    tcsetattr( FD_STDIN, TCSAFLUSH, &ttyOldStdIn );
    exit(EXIT_FAILURE);
  }

  fdUART = open( argv[1], O_RDWR | O_NOCTTY | O_NDELAY ); // Abre la UART.
  if( fdUART == -1 )
  {
    dprintf(FD_STDERR, "ERROR: no se pudo abrir %s. \n", argv[1]);
    exit(EXIT_FAILURE);
  }

/*-------- Seteo del modo NO canónico NO bloqueante y conf. de la trama en la UART --------*/
  tcgetattr( fdUART, &ttyOldUART );         // Lee atributos de la UART.
  ttyNewUART = ttyOldUART;                  // Guarda los atributos originales.

  seteoModoNoCanonicoNoBloqueante( fdUART, &ttyNewUART );
  seteoTramaYBaudRate( fdUART, &ttyNewUART, 9600 );	


/*----------------------------- Menú Principal modo remoto  -------------------------------*/
  while( opcion != 'k' )
  {
    imprimeMenu( );
    if( modoLocal == false )
        opcion = seleccionMenuModoRemoto( fdUART );
    else
      opcion = seleccionMenuModoLocal( );

    dprintf(FD_STDOUT, "\n\n");

    switch( opcion)
    {
      case 'a': // Modo remoto/local.
        dprintf(FD_STDOUT, "--------------------------------------------------------------"
                           "---------\n"
                           "Modo de control:\n"
                           "\t 1) Local\n"
                           "\t 2) Remoto\n\n"
                           "Por favor, elija el modo: ");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        modoLocal = seleccionModo( fdUART, modoLocal );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'c': // Secuencia El Auto Fantástico.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Auto Fantástico\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'd': // Secuencia El Choque.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Choque\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'e': // Secuencia La Apilada.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"La Apilada\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'f': // Secuencia La Carrera.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"La Carrera\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'g': // Secuencia El Vúmetro.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Vúmetro\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'h': // Secuencia Juntos Por Paridad.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"Juntos Por Paridad\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      case 'i': // Secuencia La Gran Moisés.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"La Gran Moisés\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
        
      case 'j': // Secuencia El Parpadeo.
        dprintf(FD_STDOUT,
               "-----------------------------------------------------------------------\n"
               "\"El Parpadeo\" en ejecución (presione 's' para volver al menú)\n");
        usleep(1000);  // Retardo que permite que se imprima el mensaje siempre.
      
        secuencia( fdUART );
      
        dprintf(FD_STDOUT, "\n\n");
        break;
      
      default:
        break;
    } // Fin del switch case.
  }


/*----------------------------- Seteo de modo por defecto (original) ----------------------*/
  tcsetattr( FD_STDIN, TCSANOW, &ttyOldStdIn ); // Actualiza los atributos del teclado 
                                                //con los valores previos.
  tcsetattr( fdUART, TCSANOW, &ttyOldUART ); // Actualiza los atributos de la UART
                                             //con los valores previos.
	
/*--------------------------- Cierre del puerto serie (UART) ------------------------------*/
  if( close(fdUART)==-1 )
    dprintf(FD_STDERR, "ERROR: no se puedo cerrar el puerto serial\n");


  return 0;
}


