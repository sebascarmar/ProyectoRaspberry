#include "projectFunctions.h"

/*******************************************************************************************/
/*******************************************************************************************/

void seteoModoNoCanonico( struct termios *t_newStdIn )
{
  t_newStdIn->c_lflag &= ~(ECHO | ICANON);    // Anula entrada canónica y eco.
  tcsetattr( FD_STDIN, TCSANOW, t_newStdIn ); // Setea los valores nuevos de la config.
}

/*******************************************************************************************/
/*******************************************************************************************/

void seteoModoNoBloqueante( struct termios *t_newStdIn )
{
  t_newStdIn->c_cc[VMIN] = 0;    // No espera a recibir ningún caracter.
  t_newStdIn->c_cc[VTIME] = 0;   // No espera tiempo alguno a recibir ningún caracter
  tcsetattr( FD_STDIN, TCSANOW, t_newStdIn ); // Setea los valores nuevos de la config.
}

/*******************************************************************************************/
/*******************************************************************************************/

void seteoModoCanonico( struct termios *t_oldStdIn, struct termios *t_newStdIn )
{
  t_newStdIn->c_lflag = t_oldStdIn->c_lflag; // Anula entrada canónica y eco.
  tcsetattr( FD_STDIN, TCSANOW, t_newStdIn ); // Setea los valores nuevos de la config.
}

/*******************************************************************************************/
/*******************************************************************************************/

void seteoModoBloqueante( struct termios *t_oldStdIn, struct termios *t_newStdIn )
{
  t_newStdIn->c_cc[VMIN] = t_oldStdIn->c_cc[VMIN];    // No espera a recibir ningún caracter.
  t_newStdIn->c_cc[VTIME] = t_oldStdIn->c_cc[VTIME];  // No espera tiempo alguno tampoco.
  tcsetattr( FD_STDIN, TCSANOW, t_newStdIn ); // Setea los valores nuevos de la config.
}

/*******************************************************************************************/
/*******************************************************************************************/

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

/*******************************************************************************************/
/*******************************************************************************************/

int velocidadSecuenciasConPote( struct termios *t_oldStdIn, struct termios *t_newStdIn )
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

  dprintf(FD_STDOUT, "\t- Velocidad (min 1, max 10):   ");
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
    retardo( 100000000 );
  }

  
  printf("\n\n");
  return velocidad;

}

/*******************************************************************************************/
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
/*******************************************************************************************/

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
  //printf("=================================================================================\n"
  //       "==                             MENÚ PRINCIPAL                                  ==\n"
  //       "=================================================================================\n"
  //       "\t\ta) Selección de modo local/remoto\n"
  //       "\t\tb) Seteo de velocidad con el potenciómetro del ADC\n"
  //       "\t\tc) Secuencia \"El Auto Fantástico\"\n"
  //       "\t\td) Secuencia \"El Choque\"\n"
  //       "\t\te) Secuencia \"La Apilada\"\n"
  //       "\t\tf) Secuencia \"La Carrera\"\n"
  //       "\t\tg) Secuencia \"El Parpadeo\"\n"
  //       "\t\th) Secuencia \"El Vúmetro\"\n"
  //       "\t\ti) Secuencia \"Sin nombre aun\"\n"
  //       "\t\tj) Secuencia \"Sin nombre aun\"\n"
  //       "\t\tk) Salir\n\n");
}

/*******************************************************************************************/
/*******************************************************************************************/

long unsigned int valorDeRetardo( int velSecuencia )
{
  switch( velSecuencia )
  {
    case 1:
      return 190000000;
    case 2:
      return 170000000;
    case 3:
      return 150000000;
    case 4:
      return 130000000;
    case 5:
      return 110000000;
    case 6:
      return 90000000;
    case 7:
      return 70000000;
    case 8:
      return 50000000;
    case 9:
      return 30000000;
    case 10:
      return 10000000;
    default:
      return 210000000;
  }
}

/*******************************************************************************************/
/*******************************************************************************************/

void imprimeVelocidadDurante( int velSecuencias )
{
      if( velSecuencias == 10)
        dprintf(FD_STDOUT, "\b\b  \b\b%d", velSecuencias);
      else
        dprintf(FD_STDOUT, "\b\b  \b\b %d", velSecuencias);
}

/*******************************************************************************************/
/*******************************************************************************************/

void secAutoFantastico( int *leds, int size, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read().

  while( buf[0] != 's' )
  {
    for(int i = 0 ; (buf[0] != 's') && (i < 8) ; i++) // Act/Desact los leds en un sentido.
    {                                                  
      digitalWrite( leds[i], 1 );
      imprimeVelocidadDurante( *velSecuencias );
      retardo( valorDeRetardo(*velSecuencias) );
      digitalWrite( leds[i], 0 );
      
      if( modoLocal == '1' ) // Lectura del teclado.
      {
        read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
        tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no transmitidos.
      }
      else
      {
        //lectura de puerto serial
      }
      
      imprimeVelocidadDurante( *velSecuencias );
    }
  
    for(int i = 6 ; (buf[0] != 's') && (i > 0) ; i--) // Act/Desact ls leds en otro sentido.
    {                                                     
      digitalWrite( leds[i], 1 );
      imprimeVelocidadDurante( *velSecuencias );
      retardo( valorDeRetardo(*velSecuencias) );
      digitalWrite( leds[i], 0 );
      
      if( modoLocal == '1' ) // Lectura del teclado.
      {
        read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
        tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no trasmitidos.
      }
      else
      {
        //lectura de puerto serial
      }
      
      imprimeVelocidadDurante( *velSecuencias );
    }

    velocidadSecuenciasConTeclado( velSecuencias, buf );

    imprimeVelocidadDurante( *velSecuencias );
  } // Fin del while.

}

/*******************************************************************************************/
/*******************************************************************************************/

void secChoque( int *leds, int size, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'}; // Almacena lo leído por que lee read().
  int i, j;             // Los leds se recorren en un sentido con i y  con j en el otro.

  while( buf[0] != 's' )
  {
    i = 0;
    j = 7;
    while( (buf[0] != 's') && (i < 8) && (j >= 0) ) // Act/Desact los leds en un sentido.
    {                                                  
      digitalWrite( leds[i], 1 );
      digitalWrite( leds[j], 1 );
      
      imprimeVelocidadDurante( *velSecuencias );
      retardo( valorDeRetardo(*velSecuencias) );
      
      digitalWrite( leds[i], 0 );
      digitalWrite( leds[j], 0 );
      
      if( modoLocal == '1' ) // Lectura del teclado.
      {
        read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
        tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no transmitidos.
      }
      else
      {
        //lectura de puerto serial
      }
      
      imprimeVelocidadDurante( *velSecuencias );
      i++;
      j--;
    }

    velocidadSecuenciasConTeclado( velSecuencias, buf );

    imprimeVelocidadDurante( *velSecuencias );
  } // Fin del while.

}

/*******************************************************************************************/
/*******************************************************************************************/

void secApilada( int *leds, int size, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'};                          // Almacena lo leído por que lee read().
  int8_t laCarrera[68][8] = { {0,0,0,0,0,0,0,0}, // Arreglo bidimensional con la rutina.
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
        imprimeVelocidadDurante( *velSecuencias );
        digitalWrite( leds[j], laCarrera[i][j] ); // Muestra en los leds la tabla.
        usleep(10000); // Delay entre cada led.
      
        if( modoLocal == '1' ) // Lectura del teclado.
        {
          read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
          tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no trasmitidos.
        }else
        {
          //lectura de puerto serial
        }
      
        velocidadSecuenciasConTeclado( velSecuencias, buf );
      }

      retardo( valorDeRetardo(*velSecuencias) );
    }

  }
  
  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWrite( leds[i], 0 );

}

/*******************************************************************************************/
/*******************************************************************************************/

void secCarrera( int *leds, int size, int8_t *velSecuencias, char modoLocal, int fdPuertoSerial )
{
  char buf[4] = {'\0'};                       // Almacena lo leído por que lee read().
  int laCarrera[17][8] = { {0,0,0,0,0,0,0,0}, // Arreglo bidimensional con la rutina.
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
        imprimeVelocidadDurante( *velSecuencias );
        digitalWrite( leds[j], laCarrera[i][j] ); // Muestra en los leds la tabla.
        usleep(10000); // Delay entre cada led.
      
        if( modoLocal == '1' ) // Lectura del teclado.
        {
          read( FD_STDIN, buf, 3 ); // read() retorna la cantidad de caracteres que lee.
          tcflush(FD_STDIN, TCIOFLUSH); // Descarta datos escritos pero no trasmitidos.
        }else
        {
          //lectura de puerto serial
        }
      
        velocidadSecuenciasConTeclado( velSecuencias, buf );
      }
      
      retardo( valorDeRetardo(*velSecuencias) );
    }

  }
  
  for(int i = 0 ; i < 8 ; i++) // Apaga todos los leds antes de volver al menú.
      digitalWrite( leds[i], 0 );

}

/*******************************************************************************************/
/*******************************************************************************************/

