.text
.arm
.global retardo

// R0: a

retardo:
    loop:
        CMP R0, #0
        BEQ salir
        SUB R0, R0, #1
        B   loop
        
    salir:
      MOV PC, LR
