.data
enunciado: .ascii "Por favor, ingrese una opción: "
seleccion: .ascii "...................."
msjError: .ascii "Valor inválido. "

.arm
.text
.global seleccionMenuModoLocal


seleccionMenuModoLocal:
    PUSH {LR}          // Apila reg. en el stack
      
    loop:
        /**************** IMPRIME EN PANTALLA EL ENUNCIADO ********************/
        MOV   R7, #4      // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1      // Descriptor de archivo de stdout (monitor).
        LDR   R1, =enunciado // Buffer de salida
        MOV   R2, #32     // Cantidad de char a escribir.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.
        
        /*********** ESPERA POR EL INGRESO DE UNA OPCION **********************/
        MOV   R7, #3      // 3 es el código de la llamada a 'read' del sistema.
        MOV   R0, #0      // Descriptor de archivo de stdin (teclado).
        LDR   R1, =seleccion // Buffer de entrada.
        MOV   R2, #20     // Cantidad de char a leer.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.
        
        /************************** SWITCH CASE *******************************/
        LDRB  R2, [R1]      // Compara el 1er char, que es el que importa.
        LDRB  R3, [R1, #1]  // Compara el 2do char, debe ser un enter.
       
    //opcion1
        CMP   R2, #'a'
        CMPNE R2, #'A'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion2
        MOV   R0, #'a'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
       
    opcion2:
        CMP   R2, #'b'
        CMPNE R2, #'B'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion3
        MOV   R0, #'b'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion3:
        CMP   R2, #'c'
        CMPNE R2, #'C'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion4
        MOV   R0, #'c'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion4:
        CMP   R2, #'d'
        CMPNE R2, #'D'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion5
        MOV   R0, #'d'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
     
    opcion5:
        CMP   R2, #'e'
        CMPNE R2, #'E'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion6
        MOV   R0, #'e'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion6:
        CMP   R2, #'f'
        CMPNE R2, #'F'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion7
        MOV   R0, #'f'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion7:
        CMP   R2, #'g'
        CMPNE R2, #'G'
        CMPEQ   R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion8
        MOV   R0, #'g'    // Valor de retorno de la función.
        B   break         // Sale del switch case.

    opcion8:
        CMP   R2, #'h'
        CMPNE R2, #'H'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion9
        MOV   R0, #'h'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion9:
        CMP   R2, #'i'
        CMPNE R2, #'I'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion10
        MOV   R0, #'i'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion10:
        CMP   R2, #'j'
        CMPNE R2, #'J'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcion11
        MOV   R0, #'j'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
      
    opcion11:
        CMP   R2, #'k'
        CMPNE R2, #'K'
        CMPEQ R3, #'\n'   // El 2do char ingresado debe ser '\n'. Por prolijidad.
        BNE opcionDefecto
        MOV   R0, #'k'    // Valor de retorno de la función.
        B   break         // Sale del switch case.
     
    opcionDefecto:
        MOV   R7, #4      // 4 es el código de la llamada a 'write' del sistema.
        MOV   R0, #1      // Descriptor de archivo de stdout (monitor).
        LDR   R1, =msjError // Buffer de salida
        MOV   R2, #17     // Cantidad de char a escribir.
        SWI   0           // Llamada al sistema para arm 32-bit/EABI.
        B   loop
     
    break:
        POP   {LR}        // Desapila reg. del stack.
        MOV   PC, LR
    
    
    
