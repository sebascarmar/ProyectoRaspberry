#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "projectFunctions.h"

extern char seleccionMenuModoLocal( void );


int main( int argc, char *argv[] )
{
  int fdPuertoSerial;     // Almacena el descriptor de archivos del puerto serie.
  struct termios ttyOldStdIn, ttyNewStdIn; // Estructuras para atributos del teclado.
  int leds[8] = {23, 24, 25, 12, 16, 20, 21, 26}; // Arreglo que contiene los leds.
  int8_t velSecuencias = 1;
  char modoLocal = '1';
  char opcion = '\0';


/*------------------- Seteo del modo NO canónico en la ENTRADA ESTANDAR -------------------*/
  tcgetattr( FD_STDIN, &ttyOldStdIn );          // Lee atributos por defecto del teclado.
  ttyNewStdIn = ttyOldStdIn;                     // Guarda los atributos originales.
  seteoModoNoCanonico( &ttyNewStdIn );


/*-------------------------------- Control de acceso --------------------------------------*/
  printf("-----------------------------------------------------------------------\n"
         "Para continuar, por favor ingrese su contraseña\n");
  if( controlDeContraseña() == 1)
  {
    printf("\n\n\t\t\t ¡BIENVENIDO AL SISTEMA!\n");
  } else
  {
    printf("\n\t\t\t NO SE HA PODIDO INGRESAR AL SISTEMA\n");
    tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
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
    printf("Error al abrir \\dev\\ttyS0\n");
    tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
                                               //con los valores originales.
    exit(EXIT_FAILURE);
  }

/*---------------------- Mapeo de pines según WiringPi (UART) -----------------------------*/
  if ( wiringPiSetup() == -1 ) // Inicializa los pines siguiendo el esquema de WiringPi.
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

  seteoModoNoBloqueante( &ttyNewStdIn );
  velSecuencias = velocidadSecuenciasConPote( &ttyOldStdIn, &ttyNewStdIn );
  seteoModoBloqueante( &ttyOldStdIn, &ttyNewStdIn );


/*-------------------------------- Menú principal -----------------------------------------*/
  while( opcion != 'k' )
  {
    tcsetattr( FD_STDIN,TCSANOW,&ttyOldStdIn ); // Setea los valores por defec. de la config.
    imprimeMenu();
    if( modoLocal == '1' )
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
      case 'a': // Modo remoto/local.
        dprintf(FD_STDOUT, "--------------------------------------------------------------"
                           "---------\n"
                           "Seleccione el modo de control:\n"
                           "\t 0) Remoto\n"
                           "\t 1) Local\n"
                           "Modo: ");
        read(FD_STDIN,&modoLocal, 1);
        tcflush(FD_STDIN, TCIOFLUSH);
      
        while( (modoLocal != '0') && (modoLocal != '1') )
        {
          dprintf(FD_STDOUT, "Opción inválida. Elija el modo: ");
          read(FD_STDIN,&modoLocal, 1);
          tcflush(FD_STDIN, TCIOFLUSH);
        }
      
        printf("\n");
        break;
      
      case 'b': // Velocidad con pote.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "Seleccione la velocidad de las secuencias con el potenciómetro del ADC\n"
               "(mín=1, máx=10)\n");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        velSecuencias = velocidadSecuenciasConPote( &ttyOldStdIn, &ttyNewStdIn );
        break;
      
      case 'c': // Secuencia El Auto Fantástico.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"El Auto Fantástico\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secAutoFantastico( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
      
      case 'd': // Secuencia El Choque.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"El Choque\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secChoque( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
      
      case 'e': // Secuencia La Apilada.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"La Apilada\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secApilada( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
      
      case 'f': // Secuencia La Carrera.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"La Carrera\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secCarrera( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
      
      case 'g': // Secuencia El Vúmetro.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"El Vúmetro\"en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secVumetro( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
      
      case 'h': // Secuencia Juntos Por Paridad.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"Juntos Por Paridad\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secJuntosPorParidad( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
      
      case 'i': // Secuencia La Gran Moisés.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"La Gran Moisés\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secGranMoises( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
        break;
        
      case 'j': // Secuencia El Parpadeo.
        dprintf(FD_STDIN,
               "-----------------------------------------------------------------------\n"
               "\"El Parpadeo\" en ejecución (presione 's' para volver al menú)\n"
               "Velocidad actual:   ");
      
        seteoModoNoCanonico( &ttyNewStdIn );
        seteoModoNoBloqueante( &ttyNewStdIn );
        secParpadeo( leds, &velSecuencias, modoLocal, fdPuertoSerial );
      
        printf("\n\n");
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
  tcsetattr(FD_STDIN, TCSANOW, &ttyOldStdIn); // Actualiza los atributos del teclado 
                                             //con los valores originales.

  return 0;
}
