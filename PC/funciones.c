#include "funciones.h"


/*#########################################################################################*/
/*##                           CONFIGURACIÓN DE LAS TTY's                                ##*/
/*#########################################################################################*/

void seteoModoNoCanonicoNoBloqueante( int fd, struct termios *ttyNew )
{
  ttyNew->c_lflag &= ~(ECHO | ICANON);    // Anula entrada canónica y eco.
  ttyNew->c_cc[VMIN] = 0;    // No espera a recibir ningún caracter.
  ttyNew->c_cc[VTIME] = 0;   // No espera tiempo alguno a recibir ningún caracter

  tcsetattr( fd, TCSAFLUSH, ttyNew );
}

/*******************************************************************************************/

void seteoTramaYBaudRate( int fd, struct termios *ttyNew, speed_t baudRate )
{
  cfsetospeed( ttyNew, baudRate ); // Velocidad de comunicación de salida.
  cfsetispeed( ttyNew, baudRate ); // Velocidad de comunicación de entrada.
  
  ttyNew->c_cflag = ( ttyNew->c_cflag & ~CSIZE ) | CS8; // 8 bits de datos (8)
  ttyNew->c_cflag &= ~( PARENB | PARODD );	        // sin paridad     (N)
  ttyNew->c_cflag &= ~CSTOPB;			        // 1 bit de stop   (1)

  ttyNew->c_iflag |= IGNCR;  // Ignora el caracter '\r' en la entrada.


  tcsetattr( fd, TCSAFLUSH, ttyNew );
}


/*#########################################################################################*/
/*##                           FUNCIONES PARA EL MENÚ                                    ##*/
/*#########################################################################################*/

void imprimeMenu( void )
{
  printf("=================================================================================\n"
         "==                        MENÚ PRINCIPAL (modo remoto)                         ==\n"
         "=================================================================================\n"
         "  a) Selección de modo local/remoto\t      g) Secuencia \"El Vúmetro\"\n"
         "  b) -                                        h) Secuencia \"Juntos Por Paridad\"\n"
         "  c) Secuencia \"El Auto Fantástico\"\t      i) Secuencia \"La Gran Moisés\"\n"
         "  d) Secuencia \"El Choque\"\t\t      j) Secuencia \"El Parpadeo\"\n"
         "  e) Secuencia \"La Apilada\"\t\t      k) SALIR\n"
         "  f) Secuencia \"La Carrera\"\n\n");
}

/*******************************************************************************************/

char seleccionMenuModoRemoto( int fdUART )
{
  char opcion[4] = {'\0'};

  while( (opcion[0] < 'a') || (opcion[0] > 'k') )
  {
    dprintf(FD_STDOUT, "Por favor, ingrese una opción vía UART: ");

    while( read(FD_STDIN, opcion, 3) == 0 ){}//Espera por ingreso por teclado.
      //tcflush(FD_STDIN, TCIOFLUSH);//Descarta lo escrito pero no transmtido (limpia buffer).

    write( fdUART, opcion, 1  ); // Envía el caracter por puerto serie.
    tcdrain(fdUART); // Espera a que lo que se haya escrito en "fdUART" se transmita.
    tcflush(fdUART, TCIOFLUSH);

    if( (opcion[0] >= 32) && (opcion[0] <=126) )//Imprime solo char imprimibles.
      dprintf(FD_STDOUT, "%c", opcion[0]); 

    if( (opcion[0] >= 'A') && (opcion[0] <= 'Z') ) // Si las letras son mayúsculas,
      opcion[0] += 32;                             //las pasa a minúsculas.

    if( (opcion[0] < 'a') || (opcion[0] > 'k') ) // Imprime msj de error si el caracter
      dprintf(FD_STDOUT, "\nValor inválido. ");  //es inválido.
  }

  return opcion[0];

}

/*******************************************************************************************/

char seleccionMenuModoLocal( void )
{
  char opcion[4] = {'\0'};

  while( opcion[0] != 'a' )
  {
    dprintf(FD_STDOUT,
        "\nSe encuentra en modo local. Cambie a modo remoto antes de continuar: ");
    
    while( read(FD_STDIN, opcion, 3) == 0 ){}//Espera por ingreso por teclado.
    
    if( (opcion[0] >= 32) && (opcion[0] <=126) )//Imprime solo char imprimibles
      dprintf(FD_STDOUT, "%c", opcion[0]); 
  }

  return opcion[0];

}


/*#########################################################################################*/
/*##                           ELECCIÓN DE MODO LOCAL O REMOTO                           ##*/
/*#########################################################################################*/

bool seleccionModo( int fdUART, bool modoLocal )
{
  char modoLocalFlag[4] = {'2','2','2','\0'};// Permite almacenar lo ingresado por teclado.

  do
  {
    while( read(FD_STDIN, modoLocalFlag, 3) == 0 ){}//Espera por ingreso por teclado.
      //tcflush(FD_STDIN, TCIOFLUSH);//Descarta lo escrito pero no transmtido (limpia buffer).

    if( modoLocal == false )
    {
      write( fdUART, modoLocalFlag, 1  ); // Envía el caracter por puerto serie.
      tcdrain(fdUART); // Espera a que lo que se haya escrito en "fdUART" se transmita.
      tcflush(fdUART, TCIOFLUSH);
    }

    if( (modoLocalFlag[0] >= 32) && (modoLocalFlag[0] <=126) )//Imprime solo char imprimibles
      dprintf(FD_STDOUT, "%c", modoLocalFlag[0]); 
  
    if( (modoLocalFlag[0] != '1') && (modoLocalFlag[0] != '2') )
      dprintf(FD_STDOUT, "\nOpción inválida. Por favor, elija el modo vía UART: ");//Mensaje de error.
    
  }while( (modoLocalFlag[0] != '1') && (modoLocalFlag[0] != '2') );//Control valores válidos.


  if(modoLocalFlag[0] == '1') // Retorna si se trata de modo local o remoto.
    return true;
  else
    return false ;
}


/*#########################################################################################*/
/*##                                SECUENCIAS                                           ##*/
/*#########################################################################################*/

void secuencia( int fdUART )
{
  char buf[4] = {'\0'};
  int datosLeidos = 0;

  while( buf[0] != 's' )
  {
    datosLeidos = read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
    //tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no transmitidos.

    if( datosLeidos != 0 )
    { 
      write( fdUART, buf, 3 ); // Envía el caracter por puerto serie.
      tcdrain(fdUART); // Espera a que lo que se haya escrito en "fdUART" se transmita.
      tcflush(fdUART, TCIOFLUSH);
    }
  }

}


