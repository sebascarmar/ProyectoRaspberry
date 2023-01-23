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
        
        LDR   R4, [R1]           // Almacena el char ingresado en R4 (reg. p/ switch).
      
      
        /** IMPRIME EN PANTALLA LO INGRESADO (SOLO CHAR IMPRIMIBLES: de ' ' hasta '~')**/
        CMP   R4, #32            // Código ASCII del caracter espacio.
        BLT   opcion1
        CMP   R4, #126           // Código ASCII del caracter virgulilla.
        BGT   opcion1
        MOV   R7, #4             // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1             // Descriptor de archivo de stdout (monitor).
        LDR   R1, =bufferIngreso // Buffer de salida
        MOV   R2, #1             // Cantidad de char a escribir.
        SWI   0                  // Llamada al sistema para arm 32-bit/EABI.
      
      
        /******************************** SWITCH CASE **********************************/
        opcion1: // Modo local/remoto.
            CMP   R4, #'a'
            CMPNE R4, #'A'
            BNE opcion2
            MOV   R0, #'a'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion2: // Velocidad con pote.
            CMP   R4, #'b'
            CMPNE R4, #'B'
            BNE opcion3
            MOV   R0, #'b'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion3: // Sec. Auto Fantástico.
            CMP   R4, #'c'
            CMPNE R4, #'C'
            BNE opcion4
            MOV   R0, #'c'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion4: // Sec. Choque.
            CMP   R4, #'d'
            CMPNE R4, #'D'
            BNE opcion5
            MOV   R0, #'d'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion5: // Sec. Apilada.
            CMP   R4, #'e'
            CMPNE R4, #'E'
            BNE opcion6
            MOV   R0, #'e'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion6: // Sec. Carrera.
            CMP   R4, #'f'
            CMPNE R4, #'F'
            BNE opcion7
            MOV   R0, #'f'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion7: // Sec. Vúmetro.
            CMP   R4, #'g'
            CMPNE R4, #'G'
            BNE opcion8
            MOV   R0, #'g'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion8: // Sec. Juntos por Paridad.
            CMP   R4, #'h'
            CMPNE R4, #'H'
            BNE opcion9
            MOV   R0, #'h'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion9: // Sec. Gran Moisés.
            CMP   R4, #'i'
            CMPNE R4, #'I'
            BNE opcion10
            MOV   R0, #'i'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion10: // Sec. Parpadeo.
            CMP   R4, #'j'
            CMPNE R4, #'J'
            BNE opcion11
            MOV   R0, #'j'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcion11: // Salir del programa.
            CMP   R4, #'k'
            CMPNE R4, #'K'
            BNE opcionDefecto
            MOV   R0, #'k'    // Valor de retorno de la función.
            B   break         // Sale del switch case.
        
        opcionDefecto:
            MOV   R7, #4        // 4 es el código de la llamada a 'write' del sistema.
            MOV   R0, #1        // Descriptor de archivo de stdout (monitor).
            LDR   R1, =msjError // Buffer de salida
            MOV   R2, #18       // Cantidad de char a escribir.
            SWI   0             // Llamada al sistema para arm 32-bit/EABI.
            B   loop
        
        
    /****************************** SALIDA DE LA FUNCIÓN *******************************/
    break:
        POP   {R4, R7, LR}  // Desapila registros del stack.
        MOV   PC, LR


