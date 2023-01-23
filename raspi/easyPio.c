/*#########################################################################################*/
/*##                               Inclusión de headers                                  ##*/
/*#########################################################################################*/
#include "easyPio.h"

// Punteros  que serán mapeados en memeoria cuando se llama a pioInit().
volatile unsigned int       *gpio; // Puntero a la base de GPIO.

/*#########################################################################################*/
/*##                               Funciones generales                                   ##*/
/*#########################################################################################*/

void pioInit( )
{
  int mem_fd;
  void *reg_map;
  mem_fd = open("/dev/mem", O_RDWR | O_SYNC); // /dev/mem es un pseudo_driver para
                                              //acceso a memoria en Linux.
  if( mem_fd < 0)                             // Verifica si se pudo abrir el arch.
  {
    printf("Error al abrir /dev/mem.\n");
    exit(EXIT_FAILURE);
  }

  reg_map = mmap(
                 NULL,                  //Dir. desde la cual inicia el mapeo local.
                 BLOCK_SIZE,            //Tamaño del bloque de mem. map. (4kB).
                 PROT_READ | PROT_WRITE,//Habilita escr./lect. para la mem. map.
                 MAP_SHARED,            //Acceso no exclusivo a esta memoria.
                 mem_fd,                //Puntero a /dev/mem.
                 GPIO_BASE);            //Offset al periférico GPIO.
  if (reg_map == MAP_FAILED)            //Verifica si se pudo realizar el mapeo.
  {
    printf("Error al  mapear GPIO  %d\n", (int)reg_map);
    close(mem_fd);
    exit(EXIT_FAILURE);
  }

  gpio = (volatile unsigned *)reg_map; // Asigna mediante un cast el bloque de mem.
                                       //mapeada y configurada.
  close(mem_fd);
}


/*#########################################################################################*/
/*##                                 Funcionetes GPIO                                    ##*/
/*#########################################################################################*/

void pinModeEP( int pin, int function ) // Los reg. GPFSEL setean la fun. de c/pin.
{
  int reg    = pin/10;     // 10 GPIO por registro (es decir, elige el registro).
  int offset = (pin%10)*3; // 3 pines para cada GPIO (selecciona los pines corres-
                           //pondientes).
  
  GPFSEL[reg]  &= ~( (0b111 & ~ function) << offset ); // Setea los ceros.
  GPFSEL[reg]  |= ( (0b111 &  function) << offset );   // Setea los unos.
}

/*******************************************************************************************/

void digitalWriteEP( int pin, int val )
{
  int reg    = pin/32;     // 10 GPIO por registro (es decir, elige el registro).
  int offset = pin%32; // 3 pines para cada GPIO (selecciona los pines correspon.).

  if(val)
    GPSET[reg] = 1 << offset; // Escribe si es HIGH.
  else
    GPCLR[reg] = 1 << offset; // Escribe si es LOW.
}

/*******************************************************************************************/

int digitalReadEP( int pin )
{
  int reg    = pin/32;
  int offset = pin%32;

  return ( GPLEV[reg] >> offset ) & 0x00000001; // Pone en cero todos los bits que
						//no interesan.
}

