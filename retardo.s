.text
.arm
.global retardo

// R0: a

retardo:
  loop:
    cmp R0, #0
    beq salir
    sub R0, R0, #1
    b   loop

  salir:
    mov pc, lr
