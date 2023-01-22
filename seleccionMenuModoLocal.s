.data
enunciado: .ascii "Por favor, ingrese una opción: "
seleccion: .ascii "\0\0\0\0"
msjError: .ascii "\nValor inválido. "

.arm
.text
.global seleccionMenuModoLocal


seleccionMenuModoLocal:
    PUSH {R4, R7, LR}          // Apila reg. en el stack
      
        /**************** IMPRIME EN PANTALLA EL ENUNCIADO ********************/
        MOV   R7, #4      // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1      // Descriptor de archivo de stdout (monitor).
        LDR   R1, =enunciado // Buffer de salida
        MOV   R2, #32     // Cantidad de char a escribir.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.
        
    loop:
        /********************** VACÍA EL BUFFER *******************************/
        MOV   R0, #'\0'
        LDR   R1, =seleccion // Buffer de entrada.
        STR   R0, [R1]


        /*********** ESPERA POR EL INGRESO DE UNA OPCION **********************/
    bucleWrite:
        MOV   R7, #3      // 3 es el código de la llamada a 'read' del sistema.
        MOV   R0, #0      // Descriptor de archivo de stdin (teclado).
        LDR   R1, =seleccion // Buffer de entrada.
        MOV   R2, #3     // Cantidad de char a leer.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.
        LDR   R4, [R1] // Almacena el caracter ingresado en R4.
        CMP   R0, #0
        BEQ   bucleWrite

        /**************** IMPRIME EN PANTALLA LO INGRESADO *********************/
        CMP   R4, #'\0' 
        BEQ   switchCase
        CMP   R4, #32
        BLT   switchCase
        CMP   R4, #126
        BGT   switchCase
        MOV   R7, #4      // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1      // Descriptor de archivo de stdout (monitor).
        LDR   R1, =seleccion // Buffer de salida
        MOV   R2, #1    // Cantidad de char a escribir.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.



        /************************** SWITCH CASE *******************************/
    switchCase:
        //LDRB  R2, [R1]      // Compara el 1er char, que es el que importa.
        MOV   R2, R4
        //LDRB  R3, [R1, #1]  // Compara el 2do char, debe ser un enter.
       
    //opcion1
        CMP   R2, #'a'
        CMPNE R2, #'A'
        BNE opcion2
        MOV   R0, #'a'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
       
    opcion2:
        CMP   R2, #'b'
        CMPNE R2, #'B'
        BNE opcion3
        MOV   R0, #'b'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion3:
        CMP   R2, #'c'
        BNE opcion4
        MOV   R0, #'c'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion4:
        CMP   R2, #'d'
        BNE opcion5
        MOV   R0, #'d'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
     
    opcion5:
        CMP   R2, #'e'
        BNE opcion6
        MOV   R0, #'e'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion6:
        CMP   R2, #'f'
        BNE opcion7
        MOV   R0, #'f'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion7:
        CMP   R2, #'g'
        BNE opcion8
        MOV   R0, #'g'    // Valor de retorno de la función.
        B   break         // Sale del switch case.

    opcion8:
        CMP   R2, #'h'
        BNE opcion9
        MOV   R0, #'h'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion9:
        CMP   R2, #'i'
        BNE opcion10
        MOV   R0, #'i'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion10:
        CMP   R2, #'j'
        BNE opcion11
        MOV   R0, #'j'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion11:
        CMP   R2, #'k'
        BNE opcion12
        MOV   R0, #'k'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
     
    opcion12:
        CMP   R2, #'\0'
        BNE opcionDefecto
        B   loop    
     
    opcionDefecto:
        MOV   R7, #4      // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1      // Descriptor de archivo de stdout (monitor).
        LDR   R1, =msjError // Buffer de salida
        MOV   R2, #18     // Cantidad de char a escribir.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.
        B   loop
     
    break:
        POP   {R4, R7, LR}        // Desapila reg. del stack.
        MOV   PC, LR
    
    
    
