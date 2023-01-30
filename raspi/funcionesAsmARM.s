.data
enunciado:     .ascii "Por favor, ingrese una opción: "
bufferIngreso: .ascii "\0\0\0\0"
msjError:      .ascii "\nValor inválido. "


.arm
.text
.global seleccionMenuModoLocal
.global retardo



/*#########################################################################################*/
/*##                             FUNCIÓN: int return( int a)                              ##*/
/*#########################################################################################*/

retardo:
    bucle:
      CMP R0, #0      // R0 = a.
      BEQ salir       // Sale del bucle cuando la variable 'a' llega a cero.
      SUB R0, R0, #1  // Le resta 1 a la variable 'a'.
      B   bucle
    
    salir:
      MOV PC, LR


/*#########################################################################################*/
/*##               FUNCIÓN: char seleccionMenuModoLocal( void )                          ##*/
/*#########################################################################################*/

seleccionMenuModoLocal:
    PUSH {R4, R7, LR} // Apila registros en el stack.
    
    loop:
      /********************** IMPRIME EN PANTALLA EL ENUNCIADO ***********************/
      MOV   R7, #4          // 4 es el código de la llamada a 'write' del sistema.
      MOV   R0, #1          // Descriptor de archivo de stdout (monitor).
      LDR   R1, =enunciado  // Buffer de salida.
      MOV   R2, #32         // Cantidad de char a escribir.
      SWI   0               // Llamada al sistema para arm 32-bit/EABI.
      
      
      /****************************** VACÍA EL BUFFER ********************************/
      MOV   R0, #'\0'
      LDR   R1, =bufferIngreso // Buffer de entrada.
      STR   R0, [R1]           // Escribe '\0' en bufferIngreso[0].
      
      
      /************ ESPERA POR EL INGRESO DE UNA OPCION POR EL TECLADO ***************/
      bucleRead:
        MOV   R7, #3             // 3 es el código de la llamada a 'read' del sistema.
        MOV   R0, #0             // Descriptor de archivo de stdin (teclado).
        LDR   R1, =bufferIngreso // Buffer de entrada.
        MOV   R2, #3             // Cantidad de char a leer.
        SWI   0                  // Llamada al sistema para arm 32-bit/EABI.
        CMP   R0, #0    
        BEQ   bucleRead          // Si read devuelve 0 char leídos, sigue en el bucle.
      
      LDR   R4, [R1]           // Almacena el char ingresado en R4.
      
      
      /** IMPRIME EN PANTALLA LO INGRESADO (SOLO CHAR IMPRIMIBLES: de ' ' hasta '~')**/
      CMP   R4, #32            // Código ASCII del caracter espacio.
      BLT valorInvalido
      CMP   R4, #126           // Código ASCII del caracter virgulilla.
      BGT valorInvalido
      MOV   R7, #4             // 4 es el código de la llamada a 'write' del sistema.
      MOV   R0, #1             // Descriptor de archivo de stdout (monitor).
      LDR   R1, =bufferIngreso // Buffer de salida
      MOV   R2, #1             // Cantidad de char a escribir.
      SWI   0                  // Llamada al sistema para arm 32-bit/EABI.
      
      
      /********** CONVIERTE A MINÚSCULAS SI SON MAYÚSCULAS (A-K -> a-k) **************/
      CMP     R4, #'A'
      BLT   valorInvalido
      CMP     R4, #'K'
      ADDLE   R4, R4, #0x20
      BLE   return
      
      
      /******************* COMPARA SI SON VALORES VÁLIDOS (a-k) **********************/
      CMP   R4, #'a'
      BLT valorInvalido
      CMP   R4, #'k'
      BLE return 
      
      
      /********* CONDICIÓN PARA SEGUIR EN EL BUCLE (caracteres no válidos) ***********/
      valorInvalido:
        MOV   R7, #4        // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1        // Descriptor de archivo de stdout (monitor).
        LDR   R1, =msjError // Buffer de salida
        MOV   R2, #18       // Cantidad de char a escribir.
        SWI   0             // Llamada al sistema para arm 32-bit/EABI.
        B   loop
    
    
    /****************************** SALIDA DE LA FUNCIÓN *******************************/
    return:
      MOV   R0, R4
      POP   {R4, R7, LR}  // Desapila registros del stack.
      MOV   PC, LR


