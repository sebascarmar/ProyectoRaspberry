#include "funciones.h"


/*#########################################################################################*/
/*##                         CONFIGURACIÓN DE LA ENTRADA ESTÁNDAR                        ##*/
/*#########################################################################################*/

void seteoModoNoCanonico( struct termios *ttyNewStdIn )
{
  ttyNewStdIn->c_lflag &= ~(ECHO | ICANON);    // Anula entrada canónica y eco.
  tcsetattr( FD_STDIN, TCSANOW, ttyNewStdIn ); // Setea los valores nuevos de la config.
}

/*******************************************************************************************/

void seteoModoNoBloqueante( struct termios *ttyNewStdIn )
{
  ttyNewStdIn->c_cc[VMIN] = 0;    // No espera a recibir ningún caracter.
  ttyNewStdIn->c_cc[VTIME] = 0;   // No espera tiempo alguno a recibir ningún caracter
  tcsetattr( FD_STDIN, TCSANOW, ttyNewStdIn ); // Setea los valores nuevos de la config.
}


/*#########################################################################################*/
/*##                                  CONTROL DE INGRESO                                 ##*/
/*#########################################################################################*/

int controlDeContraseña( void )
{
  char contra[LENGTH_PSSW+1] = {'t','d','2','2','2','\0'}; // Clave de LENGTH_PSSW+1 largo.
  char password[LENGTH_PSSW+1] = {'\0'};                   // Buffer p/guardar lo que ingrsa.
  char tecla;                                              // Almacena cada char que ingresa.
  int contraCorrecta;                                      // Flag.
  int i, asteriscosImpresos;                               // Contadores.

/*----------------- Ingreso de contraseña  y procesamiento de datos -----------------------*/
  for(int numIntentos = 1 ; numIntentos <= 3 ; numIntentos++)
  {
    tecla = 0;              // Almacena cada caracter que se ingresa.
    i = 0;                  // Índice para guardar los caracteres en el buffer 'password'.
    asteriscosImpresos = 0; // Cuenta los ast. que impresos, así se borran todos si se desea.
    contraCorrecta = 0;     // Al recorrer 'password' y comparar con 'contra', si vale 
                            //LENGTH_PSSW entnonces es válida la passwd ingresada.

    printf("\t - Contraseña: ");
    while ( tecla != 10 ) // Ingresar hasta oprimir enter. Simula en parte el modo canónico.
    {
      tecla = getchar();
      
      if( tecla != 10 ) // Si no se presiona ENTER es verdadero. Si se presiona ENTER lo toma
      {                 //como un intento.
        if( tecla != 127 ) // Si NO se presiona DEL es verdadero.
        {
          if( (i <= (LENGTH_PSSW-1)) && (i == asteriscosImpresos) )// i no puede ser mayor al 
          {                                                   //largo de la contra, y debe 
                                                              //coincdr c/los astrs. impresos
            password[i] = tecla;
            i++;
          }                   
          
          printf("*");
          asteriscosImpresos++;
          
        }else            // Si SÍ se presiona DEL es verdadero.
        {
          if( asteriscosImpresos > 0 ) // Verdadero si hay asteriscos impresos en pantalla.
          {
            printf("\b \b"); // Borra los asteriscos en pantalla.
            asteriscosImpresos--;
            if( i > 0 ) // Permite que coincidan los astrscs impresos c/los elem. del array.
              i--;       
          }
        }
      } 
    }
    
    for(int j = 0 ; j < LENGTH_PSSW ; j++) // Analiza si la contraseña ingresada es correcta.
    { 
      if( contra[j] == password[j] )
        contraCorrecta++;
    }
    
    if( contraCorrecta == 5 ){
      numIntentos = 3; 
    }else
      printf("\n\t Contraseña inválida\n");
    
  } // Fin del for.


  if( contraCorrecta == 5 )
    return 1;
  else
    return 0;
}


/*#########################################################################################*/
/*##                               VELOCIDAD DE SECUENCIAS                               ##*/
/*#########################################################################################*/

int velocidadSecuenciasConPote( void )
{
  char tecla[1] = {'\0'};         // Almacena lo leído por read().
  int fdModuloADC, valorADC;      // Declaracion de variables para acceder al ADC.
  int velocidad; 

  fdModuloADC = wiringPiI2CSetup(ADDRESS); // Inicializa el sistema I2C con el ID del dispos.
  if( fdModuloADC <= -1 )
  {
    printf("Error al inicializar el systema I2C.\n");
    exit(EXIT_FAILURE);	
  }

  dprintf(FD_STDOUT, "\t- Velocidad (presione 'enter' para setear):   ");
  while( tecla[0] != 10 )
  {
    // Lectura del canal ADC0 (potenciómetro).
    wiringPiI2CReadReg8(fdModuloADC, A0); // Activa la conversión.
    valorADC = wiringPiI2CReadReg8(fdModuloADC, A0);// Lectura del valor.
    dprintf(FD_STDOUT, "\b\b  \b\b");

    if( valorADC  <= 26 ) 
      dprintf(FD_STDOUT, " %d", velocidad = 1);
    else if( (valorADC  > 26) && (valorADC <= 51) )
      dprintf(FD_STDOUT, " %d", velocidad = 2);
    else if( (valorADC  > 51) && (valorADC <= 77) )
      dprintf(FD_STDOUT, " %d", velocidad = 3);
    else if( (valorADC  > 77) && (valorADC <= 102) )
      dprintf(FD_STDOUT, " %d", velocidad = 4);
    else if( (valorADC  > 102) && (valorADC <= 128) )
      dprintf(FD_STDOUT, " %d", velocidad = 5);
    else if( (valorADC  > 128) && (valorADC <= 153) )
      dprintf(FD_STDOUT, " %d", velocidad = 6);
    else if( (valorADC  > 153) && (valorADC <= 179) )
      dprintf(FD_STDOUT, " %d", velocidad = 7);
    else if( (valorADC  > 179) && (valorADC <= 204) )
      dprintf(FD_STDOUT, " %d", velocidad = 8);
    else if( (valorADC  > 204) && (valorADC <= 230) )
      dprintf(FD_STDOUT, " %d", velocidad = 9);
    else
      dprintf(FD_STDOUT, "%d", velocidad = 10);

    read( FD_STDIN, tecla, 1 ); // Se guarda el caracter ingresado en "tecla[0]".
    retardo( 10000000 );  // Retardo que impide el parpadeo en la impresión del valor.
  }

  
  dprintf(FD_STDOUT, "\n\n");
  return velocidad;

}

/*******************************************************************************************/

void velocidadSecuenciasConTeclado( int8_t *velSecuencias, char *buf )
{
  if( (buf[2] == 'A') && (*velSecuencias < 10) )
  {
    (*velSecuencias)++;
    buf[2] = '\0';
  }
  if( (buf[2] == 'B') && (*velSecuencias > 1) )
  {
    (*velSecuencias)--;
    buf[2] = '\0';
  }
}

/*******************************************************************************************/

int valorDeRetardo( int velSecuencia )
{
  switch( velSecuencia )
  {
    case 1:
      return 100000000;
    case 2:
      return 90000000;
    case 3:
      return 80000000;
    case 4:
      return 70000000;
    case 5:
      return 60000000;
    case 6:
      return 50000000;
    case 7:
      return 40000000;
    case 8:
      return 30000000;
    case 9:
      return 20000000;
    case 10:
      return 10000000;
    default:
      return 200000000;
  }
}


/*#########################################################################################*/
/*##                           FUNCIONES PARA EL MENÚ                                    ##*/
/*#########################################################################################*/

void imprimeMenu( void )
{
  printf("=================================================================================\n"
         "==                             MENÚ PRINCIPAL                                  ==\n"
         "=================================================================================\n"
         "  a) Selección de modo local/remoto\t      g) Secuencia \"El Vúmetro\"\n"
         "  b) Velocidad con el potenciómetro del ADC   h) Secuencia \"Juntos Por Paridad\"\n"
         "  c) Secuencia \"El Auto Fantástico\"\t      i) Secuencia \"La Gran Moisés\"\n"
         "  d) Secuencia \"El Choque\"\t\t      j) Secuencia \"El Parpadeo\"\n"
         "  e) Secuencia \"La Apilada\"\t\t      k) SALIR\n"
         "  f) Secuencia \"La Carrera\"\n\n");
}

/*******************************************************************************************/

char seleccionMenuModoRemoto( int fdPuertoSerial )
{
  // Falta completar
  return 'k'; //temporal
}


/*#########################################################################################*/
/*##                           ELECCIÓN DE MODO LOCAL O REMOTO                           ##*/
/*#########################################################################################*/

bool seleccionModoEnModoLocal( void )
{
  char modoLocalFlag[4] = {'1','1','1','\0'};// Permite almacenar lo ingresado por teclado.

  do
  {
    while( read(FD_STDIN, modoLocalFlag, 3) == 0 )//Espera por ingreso por teclado.
      tcflush(FD_STDIN, TCIOFLUSH);//Descarta lo escrito pero no transmtido (limpia buffer).

    if( (modoLocalFlag[0] >= 32) && (modoLocalFlag[0] <=126) )//Imprime solo char imprimibles
      dprintf(FD_STDOUT, "%c", modoLocalFlag[0] ); 
  
    if( (modoLocalFlag[0] != '1') && (modoLocalFlag[0] != '2') )
      dprintf(FD_STDOUT, "\nOpción inválida. Por favor, elija el modo: ");//Mensaje de error.
    
  }while( (modoLocalFlag[0] != '1') && (modoLocalFlag[0] != '2') );//Control valores válidos.


  if(modoLocalFlag[0] == '1') // Retorna si se trata de modo local o remoto.
    return true;
  else
    return false ;

}

/*******************************************************************************************/

bool seleccionModoEnModoRemoto( int fdPuertoSerial )
{
  //Falta completar.
  return false; // temporal
}


/*#########################################################################################*/
/*##                   MANEJO DE STDIN Y STDOUT DURANTE SECUENCIAS                       ##*/
/*#########################################################################################*/

void imprimeVelocidadDurante( int velSecuencias )
{
      if( velSecuencias == 10)
        dprintf(FD_STDOUT, "\b\b  \b\b%d", velSecuencias);
      else
        dprintf(FD_STDOUT, "\b\b  \b\b %d", velSecuencias);
}

/*******************************************************************************************/

void lecturaTeclado( bool modoLocal, int fdPuertoSerial, char *buf ) //Modo remoto en revisión
{
  if( modoLocal == true ) // Lectura del teclado local
  {
    read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
    tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no transmitidos.
  }
  else                   // Lectura teclado remoto mediante puerto serie.
  {
    if( serialDataAvail(fdPuertoSerial) )       // Retorna el número de caracteres
    {                                           //disponible para leer.
      for(int i = 0 ; i < 3 ; i++)
        buf[i] = serialGetchar( fdPuertoSerial ); // Retorna el siguiente caracter 
                                                  //disponible en el dispositivo serial.	
      
      tcflush(fdPuertoSerial, TCIOFLUSH); // Descarta datos escritos pero no transmitidos.
    }
  }
}


/*#########################################################################################*/
/*##                          SECUENCIAS DE LUCES                                        ##*/
/*#########################################################################################*/

void secAutoFantastico( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.

  while( buf[0] != 's' )
  {
    for(int i = 0 ; (buf[0] != 's') && (i < 8) ; i++) // Act/Desact los leds en un sentido.
    {                                                  
      lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
      velocidadSecuenciasConTeclado( velSecuencias, buf );
      imprimeVelocidadDurante( *velSecuencias );
      
      digitalWriteEP( leds[i], 1 );
     
      retardo( valorDeRetardo(*velSecuencias) );
      
      digitalWriteEP( leds[i], 0 );
    }
  
    for(int i = 6 ; (buf[0] != 's') && (i > 0) ; i--) // Act/Desact ls leds en otro sentido.
    {                                                     
      lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
      velocidadSecuenciasConTeclado( velSecuencias, buf );
      imprimeVelocidadDurante( *velSecuencias );
      
      digitalWriteEP( leds[i], 1 );
      
      retardo( valorDeRetardo(*velSecuencias) );
      
      digitalWriteEP( leds[i], 0 );
    }

  } // Fin del while.

}

/*******************************************************************************************/

void secChoque( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.
  int i, j;             // Los leds se recorren en un sentido con i y  con j en el otro.

  while( buf[0] != 's' )
  {
    i = 0;
    j = 7;
    while( (buf[0] != 's') && (i < 8) && (j >= 0) ) // Act/Desact los leds en un sentido.
    {                                                  
      lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
      velocidadSecuenciasConTeclado( velSecuencias, buf );
      imprimeVelocidadDurante( *velSecuencias );
      
      digitalWriteEP( leds[i], 1 );
      digitalWriteEP( leds[j], 1 );
      
      retardo( valorDeRetardo(*velSecuencias) );
      
      digitalWriteEP( leds[i], 0 );
      digitalWriteEP( leds[j], 0 );
      
      i++;
      j--;
    }
  } // Fin del 1er while.

}

/*******************************************************************************************/

void secApilada( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.
  bool laApilada[68][8] = { {0,0,0,0,0,0,0,0}, // Arreglo bidimensional con la rutina.
                            {1,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0},
                            {0,0,1,0,0,0,0,0},
                            {0,0,0,1,0,0,0,0},
                            {0,0,0,0,1,0,0,0},
                            {0,0,0,0,0,1,0,0},
                            {0,0,0,0,0,0,1,0},
                            {0,0,0,0,0,0,0,1},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,1},
                            {0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,1},
                            {0,1,0,0,0,0,0,1},
                            {0,0,1,0,0,0,0,1},
                            {0,0,0,1,0,0,0,1},
                            {0,0,0,0,1,0,0,1},
                            {0,0,0,0,0,1,0,1},
                            {0,0,0,0,0,0,1,1},
                            {0,0,0,0,0,0,0,1},
                            {0,0,0,0,0,0,1,1},
                            {0,0,0,0,0,0,0,1},
                            {0,0,0,0,0,0,1,1},
                            {1,0,0,0,0,0,1,1},
                            {0,1,0,0,0,0,1,1},
                            {0,0,1,0,0,0,1,1},
                            {0,0,0,1,0,0,1,1},
                            {0,0,0,0,1,0,1,1},
                            {0,0,0,0,0,1,1,1},
                            {0,0,0,0,0,0,1,1},
                            {0,0,0,0,0,1,1,1},
                            {0,0,0,0,0,0,1,1},
                            {0,0,0,0,0,1,1,1},
                            {1,0,0,0,0,1,1,1},
                            {0,1,0,0,0,1,1,1},
                            {0,0,1,0,0,1,1,1},
                            {0,0,0,1,0,1,1,1},
                            {0,0,0,0,1,1,1,1},
                            {0,0,0,0,0,1,1,1},
                            {0,0,0,0,1,1,1,1},
                            {0,0,0,0,0,1,1,1},
                            {0,0,0,0,1,1,1,1},
                            {1,0,0,0,1,1,1,1},
                            {0,1,0,0,1,1,1,1},
                            {0,0,1,0,1,1,1,1},
                            {0,0,0,1,1,1,1,1},
                            {0,0,0,0,1,1,1,1},
                            {0,0,0,1,1,1,1,1},
                            {0,0,0,0,1,1,1,1},
                            {0,0,0,1,1,1,1,1},
                            {1,0,0,1,1,1,1,1},
                            {0,1,0,1,1,1,1,1},
                            {0,0,1,1,1,1,1,1},
                            {0,0,0,1,1,1,1,1},
                            {0,0,1,1,1,1,1,1},
                            {0,0,0,1,1,1,1,1},
                            {0,0,1,1,1,1,1,1},
                            {1,0,1,1,1,1,1,1},
                            {0,1,1,1,1,1,1,1},
                            {0,0,1,1,1,1,1,1},
                            {0,1,1,1,1,1,1,1},
                            {0,0,1,1,1,1,1,1},
                            {0,1,1,1,1,1,1,1},
                            {1,1,1,1,1,1,1,1},
                            {0,1,1,1,1,1,1,1},
                            {1,1,1,1,1,1,1,1},
                            {1,1,1,1,1,1,1,1}
                          }; 
  
  while( buf[0] != 's' )
  {
    for(int i = 0 ; (buf[0] != 's') && (i < 68) ; i++) 
    { 							     
      for(int j = 0 ; (buf[0] != 's') && (j < 8) ; j++)
      {
        lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
         
        velocidadSecuenciasConTeclado( velSecuencias, buf );
        imprimeVelocidadDurante( *velSecuencias );
         
        digitalWriteEP( leds[j], laApilada[i][j] ); // Muestra en los leds la tabla.
        usleep(5000); // Delay entre cada led.
      } // Fin 2º for.
        
      retardo( valorDeRetardo(*velSecuencias) );
    } // Fin 1º for.

  } // Fin del while.
  
  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWriteEP( leds[i], 0 );

}

/*******************************************************************************************/

void secCarrera( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.
  bool laCarrera[17][8] = { {0,0,0,0,0,0,0,0}, // Arreglo bidimensional con la rutina.
                            {0,0,0,0,0,0,0,0},
                            {1,0,0,0,0,0,0,0},
                            {1,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0},
                            {0,0,1,0,0,0,0,0},
                            {0,0,1,0,0,0,0,0},
                            {0,0,0,1,0,0,0,0},
                            {1,0,0,1,0,0,0,0},
                            {0,1,0,0,1,0,0,0},
                            {0,0,1,0,1,0,0,0},
                            {0,0,0,1,0,1,0,0},
                            {0,0,0,0,1,1,0,0},
                            {0,0,0,0,0,1,1,0},
                            {0,0,0,0,0,0,1,0},
                            {0,0,0,0,0,0,0,1}
                          }; 
   
  while( buf[0] != 's' )
  {
    for(int i = 0 ; (buf[0] != 's') && (i < 17) ; i++) 
    { 							     
      for(int j = 0 ; (buf[0] != 's') && (j < 8) ; j++)
      {
        lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
        
        velocidadSecuenciasConTeclado( velSecuencias, buf );
        imprimeVelocidadDurante( *velSecuencias );
        
        digitalWriteEP( leds[j], laCarrera[i][j] ); // Muestra en los leds la tabla.
        usleep(10000); // Delay entre cada led.
      } // Fin 2do for.
      
      retardo( valorDeRetardo(*velSecuencias) );
    } // Fin 1er for.

  }
  
  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWriteEP( leds[i], 0 );

}

/*******************************************************************************************/

void secVumetro( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.
  bool elVumetro[25][8] = { {0,0,0,0,0,0,0,0}, // Arreglo bidimensional con la rutina.
                            {1,1,0,0,0,0,0,0},
                            {1,1,1,1,1,0,0,0},
                            {1,1,1,1,1,1,1,0},
                            {1,1,1,0,0,0,0,0},
                            {1,1,1,1,1,1,0,0},
                            {1,1,0,0,0,0,0,0},
                            {1,1,1,1,1,0,0,0},
                            {1,1,1,1,1,1,1,1},
                            {1,1,1,1,1,1,0,0},
                            {1,1,1,1,1,1,1,0},
                            {1,1,1,1,0,0,0,0},
                            {1,1,1,1,1,1,0,0},
                            {1,1,1,1,0,0,0,0},
                            {1,1,0,0,0,0,0,0},
                            {1,0,0,0,0,0,0,0},
                            {1,1,1,0,0,0,0,0},
                            {1,1,0,0,0,0,0,0},
                            {1,1,1,1,0,0,0,0},
                            {1,1,1,1,1,0,0,0},
                            {1,1,1,1,1,1,1,1},
                            {1,1,1,1,1,1,1,0},
                            {1,1,1,1,1,1,0,0},
                            {1,1,1,1,1,1,1,1},
                            {1,1,1,1,1,0,0,0}
                          }; 
  
  while( buf[0] != 's' )
  {
    for(int i = 0 ; (buf[0] != 's') && (i < 25) ; i++) 
    { 							     
      for(int j = 0 ; (buf[0] != 's') && (j < 8) ; j++)
      {
        lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
        velocidadSecuenciasConTeclado( velSecuencias, buf );
        imprimeVelocidadDurante( *velSecuencias );
      
        digitalWriteEP( leds[j], elVumetro[i][j] ); // Muestra en los leds la tabla.
        usleep(10000); // Delay entre cada led.
      }
      
      retardo( valorDeRetardo(*velSecuencias) );
    }

  }
  
  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWriteEP( leds[i], 0 );

}

/*******************************************************************************************/

void secJuntosPorParidad( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.
  bool juntosPorParidad[34][8] = { {0,0,0,0,0,0,0,0}, //Arreglo bidimensional con la rutina.
                                   {0,1,0,0,0,0,0,0},
                                   {0,1,0,1,0,0,0,0},
                                   {0,1,0,1,0,1,0,0},
                                   {0,1,0,1,0,1,0,1},
                                   {0,0,0,0,0,0,0,0},
                                   {0,1,0,1,0,1,0,1},
                                   {0,0,0,0,0,0,0,0},
                                   {0,1,0,1,0,1,0,1},
                                   {0,0,0,0,0,0,0,0},
                                   {0,1,0,1,0,1,0,1},
                                   {0,0,0,0,0,0,0,0},
                                   {0,1,0,1,0,1,0,1},
                                   {0,1,0,1,0,1,0,1},
                                   {0,0,0,0,0,0,0,0},
                                   {0,0,0,0,0,0,1,0},
                                   {0,0,0,0,1,0,1,0},
                                   {0,0,1,0,1,0,1,0},
                                   {1,0,1,0,1,0,1,0},
                                   {0,0,0,0,0,0,0,0},
                                   {1,0,1,0,1,0,1,0},
                                   {0,0,0,0,0,0,0,0},
                                   {1,0,1,0,1,0,1,0},
                                   {0,0,0,0,0,0,0,0},
                                   {1,0,1,0,1,0,1,0},
                                   {0,0,0,0,0,0,0,0},
                                   {1,0,1,0,1,0,1,0},
                                   {1,0,1,0,1,0,1,0},
                                   {0,1,0,1,0,1,0,1},
                                   {1,0,1,0,1,0,1,0},
                                   {0,1,0,1,0,1,0,1},
                                   {1,0,1,0,1,0,1,0},
                                   {0,1,0,1,0,1,0,1},
                                   {1,0,1,0,1,0,1,0}
                                 }; 
  
  while( buf[0] != 's' )
  {
    for(int i = 0 ; (buf[0] != 's') && (i < 34) ; i++) 
    { 							     
      for(int j = 0 ; (buf[0] != 's') && (j < 8) ; j++)
      {
        lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
        velocidadSecuenciasConTeclado( velSecuencias, buf );
        imprimeVelocidadDurante( *velSecuencias );
        
        digitalWriteEP( leds[j], juntosPorParidad[i][j] ); // Muestra en los leds la tabla.
      } // Fin del 2do for.
      
      retardo( valorDeRetardo(*velSecuencias) );
    } // Fin del 1er for.

  }
  
  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWriteEP( leds[i], 0 );

}

/*******************************************************************************************/

void secGranMoises( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.
  int j, k;             // Los leds se recorren en un sentido con i y  con j en el otro.

  while( buf[0] != 's' )
  {
    lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.

    velocidadSecuenciasConTeclado( velSecuencias, buf );
    imprimeVelocidadDurante( *velSecuencias );
    
    for(int i=0 ; (buf[0] != 's') && (i < 8) ; i++) // Enciende todos los leds.
      digitalWriteEP( leds[i], 1);
    usleep(400000);
    
    j=3;
    k=4;
    while( (buf[0] != 's') &&  (j >= 1) && (k <= 6) ) // Apaga los leds desde el centro.
    {
      lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
      velocidadSecuenciasConTeclado( velSecuencias, buf );
      imprimeVelocidadDurante( *velSecuencias );
      
      digitalWriteEP( leds[j], 0 );
      digitalWriteEP( leds[k], 0 );
      
      retardo( valorDeRetardo(*velSecuencias) );
      
      j--;
      k++;
    }
  
    usleep(200000);

    while( (buf[0] != 's') && (j <= 3) && (k >= 4) ) // Enciende los leds desde los extremos.
    {
      lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.
      
      velocidadSecuenciasConTeclado( velSecuencias, buf );
      imprimeVelocidadDurante( *velSecuencias );
      
      digitalWriteEP( leds[j], 1 );
      digitalWriteEP( leds[k], 1 );
      
      retardo( valorDeRetardo(*velSecuencias) );
      
      j++;
      k--;
    }
    
  } // Fin del 1er while.

  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWriteEP( leds[i], 0 );
    
}

/*******************************************************************************************/

void secParpadeo( int *leds, int8_t *velSecuencias, bool modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read() o por el puerto serie.

  while( buf[0] != 's' )
  {
    lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.

    velocidadSecuenciasConTeclado( velSecuencias, buf );
    imprimeVelocidadDurante( *velSecuencias );
    
    for(int i=0 ; (buf[0] != 's') && (i < 8) ; i++) // Enciende todos los leds.
      digitalWriteEP( leds[i], 1);
    
    retardo( valorDeRetardo(*velSecuencias) );

    lecturaTeclado( modoLocal, fdPuertoSerial, buf ); // Lee el teclado local o remoto.

    velocidadSecuenciasConTeclado( velSecuencias, buf );
    imprimeVelocidadDurante( *velSecuencias );
    
    for(int j = 0 ; (buf[0] != 's') && (j < 8) ; j++) // Apaga todos los leds.
      digitalWriteEP( leds[j], 0 );

    retardo( valorDeRetardo(*velSecuencias) );
  } // Fin del while.
    
  for(int k = 0 ; k < 8 ; k++) // Apaga todos los leds antes de volver al menú.
      digitalWriteEP( leds[k], 0 );

}


