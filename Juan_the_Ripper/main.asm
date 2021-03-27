extern _ZN6SHA5124hashEPKc                      ;Funcion para hashear.
extern printf                                   ;Funcion para imprimir.
section .data
    mensaje db "Hola como estas",0xA,0x0
    formato db "%s",0xA,0x0
    mensajeLen equ $-mensaje

section .bss
    mensajeTextoPlano resq 1
    sha resq 1

section .text
global start
start: 
    push rbp
    mov rbp, rsp
    sub rsp, 16

    xor r8, r8
    mov [mensajeTextoPlano], rsi

    mov rsi, [mensajeTextoPlano]
    call _ZN6SHA5124hashEPKc

    mov [sha], rax 

    add rsp, 16

    sub rsp, 16

    mov rdi, formato                       ;Imprimir el borde punteado.
    mov rax, 0
    mov rsi, [sha]
    call printf

    add rsp, 16

    ;mov rax, r8
    ;mov rax, 1
    ;mov rdi, 1                          ;Llamada de systema (write), que escribe al
    ;mov rsi, r8                         ;descriptor de archivos stderr.
    ;mov rdx, 64
    ;syscall

    push rdi                            ;Guardar el valor del registro rdi.
    push rax                            ;Guardar el valor del registro rax.
    push rsi                            ;Guardar el valor del registro rsi.
    sub rsp, 8                          ;Alinear la pila.

    mov rdi, formato                       ;Imprimir el borde punteado.
    mov rax, 0
    mov rsi, [mensajeTextoPlano]
    call printf

    add rsp, 8                          ;Re-establecer la pila.
    pop rsi                             ;Re-establecer el registo rsi.
    pop rax                             ;Re-establecer el registo rax.
    pop rdi                             ;Re-establecer el registo rdi.

    mov rsp, rbp
    pop rbp
    ret