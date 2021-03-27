extern _ZN6SHA5124hashEPKc              ;Funcion para hashear.
extern printf                           ;Funcion para imprimir.
section .data
    formato db "%s",0xA,0x0             ;Formato simple para hacer debug.

section .bss
    mensajeTextoPlano resq 1            ;Almacen de la contraseña en texto plano.
    sha resq 1                          ;Almacen de la contraseña cifrada.

section .text
global start
start: 
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    ;Cifrar el texto plano.
    ;--------------------------------------------------------------------------

    mov [mensajeTextoPlano], rsi        ;Guardar el mensaje en texto plano.
    sub rsp, 16                         ;Espacio de sombra para *this, input.

    mov rsi, [mensajeTextoPlano]        ;Pasar como argumento el mensaje.
    call _ZN6SHA5124hashEPKc            ;llamar la funcion hash(char* input).
    mov [sha], rax                      ;Guardar el hash (valor de retorno).

    add rsp, 16                         ;Reestablecer la pila.

    ;--------------------------------------------------------------------------

    sub rsp, 24

    mov rax, 0
    mov rdi, formato                       ;Imprimir el borde punteado.
    mov rsi, [sha]
    call printf

    add rsp, 24

    mov rsp, rbp                        ;Reestablecer el apuntador de
    pop rbp                             ;la pila.
    ret