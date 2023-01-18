#include "projectFunctions.h"

/*------------------------------------------------------------------------------------------*/

void seteoModoNoCanonico( struct termios *t_oldStdIn, struct termios *t_newStdIn )
{
  tcgetattr( FD_STDIN, t_oldStdIn );          // Lee atributos por defecto del teclado.
  *t_newStdIn = *t_oldStdIn;                  // Guarda los atributos originales.
  t_newStdIn->c_lflag &= ~(ECHO | ICANON);    // Anula entrada canónica y eco.
  tcsetattr( FD_STDIN, TCSANOW, t_newStdIn ); // Setea los valores nuevos de la config.
}

/*------------------------------------------------------------------------------------------*/

int controlDeContraseña( void )
{
  char contra[LENGTH_PSSW+1] = {'t','d','2','2','2','\0'};  // Contraseña de LENGTH_PSSW carcateres (más el caracter NULL).
  char password[LENGTH_PSSW+1] = {'\0'};                    // Buffer para almacenar la contraseña que se ingresa.
  char tecla;                                               // Variable que almacena cada caracter que se ingresa.
  int i, asteriscosImpresos, contraCorrecta;     

  /*****************INGRESO DE CONTRASEÑA  Y PROCESAMIENTO DE DATOS************************/
  for(int numIntentos = 1 ; numIntentos <= 3; numIntentos++)
  {
    tecla = 0;              // Almacena cada caracter que se ingresa.
    i = 0;                  // Índice para guardar los caracteres en el buffer 'password'.
    asteriscosImpresos = 0; // Cuenta los ast. que se imprimen, para poder borrar todos si se quiere.
    contraCorrecta = 0;     // Al recorrer 'password' y comparar con 'contra', si vale LENGTH_PSSW entncs es válida la passwd ingresada.

    printf("\t - Contraseña: ");
    while ( tecla!=10 )    // Ingresar hasta oprimir enter. Parte de la simulación del modo canónico.
    {
      tecla = getchar();
      
      if( tecla!=10 ) // Si no se presiona ENTER es verdadero. Si se presiona ENTER, lo toma como un intento directamente.
      {
        if( tecla!=127 ) // Si NO se presiona DEL es verdadero.
        {
          if( i<=(LENGTH_PSSW-1) && i==asteriscosImpresos)// 'i' no puede ser mayor al largo de la contra, y debe coincidir con 
          {                                               //los asteriscos en pantalla.
            password[i] = tecla;
            i++;
          }                   
          
          printf("*");
          asteriscosImpresos++;
          
        }else            // Si SÍ se presiona DEL es verdadero.
        {
          if( asteriscosImpresos>0 ) // Solo borra caracteres en pantalla si fueron asteriscos impresos.
          {
            printf("\b \b"); // Imprime un backspace, sobreescribe con un espacio, y vuelve el cursor hacia atrás con otro backspace.
            asteriscosImpresos--;
            if( i>0 ) // Se puede agregar que borre solo cuando asteriscosImpresos vale 4 o menos.
              i--;       
          }
        }
      } 
    }
    
    for(int j = 0 ; j<LENGTH_PSSW ; j++) // Analiza si la contraseña ingresada es correcta.
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

/*------------------------------------------------------------------------------------------*/

int velocidadInicialSecuencias( void )
{
  char tecla[1] = {'\0'};         // Almacena lo leído por que lee read().
  int fdModuloADC, valorADC;    // Declaracion de variables para acceder al ADC.
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

/*------------------------------------------------------------------------------------------*/

void imprimeMenu( void )
{
  printf("=================================================================================\n"
         "##                             MENÚ PRINCIPAL                                  ##\n"
         "=================================================================================\n"
         "\t\ta) Selección de modo local/remoto\n"
         "\t\tb) Seteo de velocidad con el potenciómetro del ADC\n"
         "\t\tc) Secuencia \"El Auto Fantástico\"\n"
         "\t\td) Secuencia \"El Choque\"\n"
         "\t\te) Secuencia \"La Apilada\"\n"
         "\t\tf) Secuencia \"La Carrera\"\n"
         "\t\tg) Secuencia \"Sin nombre aun\"\n"
         "\t\th) Secuencia \"Sin nombre aun\"\n"
         "\t\ti) Secuencia \"Sin nombre aun\"\n"
         "\t\tj) Secuencia \"Sin nombre aun\"\n"
         "\t\tk) Salir\n\n");

}
