;--------------------------------------------------------------------------------
;Cifrado Cesar - Carlos Sebastian Madrigal Rodriguez
;23 de Marzo, 2021
;--------------------------------------------------------------------------------
extern printf
section .data
    ;int main(int argc, char** argv)
    ;variables que almacenan los argumentos pasados al 
    ;programa.
    ;------------------------------------------------------
    ;El orden de los registros: 
    ;%rdi, %rsi, %rdx, %rcx, %r8 and %r9 
    ;eso significa que argc esta en %rdi y argv en %rsi.

    %define INICIO_ASCII_MINUSCULAS 97
    %define INICIO_ASCII_MAYUSCULAS 65
    %define LONGITUD_ALFABETO 26
    %define KNRM  `\x1B[0m`
    %define KRED  `\x1B[31m`
    %define KGRN  `\x1B[32m`
    %define KYEL  `\x1B[33m`
    %define KBLU  `\x1B[34m`
    %define KMAG  `\x1B[35m`
    %define KCYN  `\x1B[36m`
    %define KWHT  `\x1B[37m`

    strLimite db KCYN,`--------------------------------------------------------`,0xA,0x0
    nombreAplicacion db KYEL,`\t\tCifrado de Cesar!\n`,0xA,0x0
    formatoRotaciones db KCYN,`El numero de desplazamientos es %d.\n`,0xA,0x0
    formatoTextoCrypt db KWHT,`El mensaje a encriptar es: `, KYEL, `%s`,0xA,0x0
    formatoTextoCrypt2 db KWHT,`El mensaje encriptado es: `, KYEL, `%s`,0xA,0x0
    formatoTextoDecrypt db KWHT,`El mensaje a desencriptar es: `, KYEL, `%s`,0xA,0x0
    formatoTextoDecrypt2 db KWHT,`El mensaje desencriptado es: `, KYEL, `%s`,0xA,0x0
    fewArgumentsError db `Error en los argumentos!.`,0xA,0x0
    fewArgumentsErrorLen equ $-fewArgumentsError
    STDOUT_FILENO equ 0x01
    STDERR_FILENO equ 0x02
    
section .bss
    argc resq 1
    nDesplazamientos resq 1
    mensaje resq 1
section .text
global main
main:
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    ;Verificar si se ingresaron los argumentos necesarios.
    ;(numero de desplazamientos y el texto a cifrar).
    ;--------------------------------------------------------------------------
    
    cmp rdi,3
    jl exit_error

    ;--------------------------------------------------------------------------

     ;Almacenar los argumentos (numero de desplazamientos y el texto a cifrar).
    ;--------------------------------------------------------------------------

    xor rax,rax                         ;Limpiamos rax.
    xor rdx, rdx                        ;Limpiamos rdx.
    mov [argc], rdi                     ;Guardamos el numero de argumentos.
    mov rax, [rsi+8]                    ;Direccion de memoria de argv[1] en r8.
    mov rdx, [rax]                      ;Desreferencia *argv[1].
    sub dl, '0'                         ;Convertimos de string a int.
    mov [nDesplazamientos], byte dl     ;Guardamos el numero de desplazamientos.

    mov r8, [rsi+24]
    mov [mensaje], r8

    ;--------------------------------------------------------------------------

    ;Imprimir marco y contexto de la aplicacion.
    ;--------------------------------------------------------------------------
    push rdi                            ;Guardar el valor del registro rdi.
    push rax                            ;Guardar el valor del registro rax.
    push rsi                            ;Guardar el valor del registro rsi.
    sub rsp, 8                          ;Alinear la pila.

    mov rdi, strLimite                  ;Imprimir el borde punteado.
    mov rax, 0
    call printf

    mov rdi, nombreAplicacion           ;Imprimir el nombre de la aplicacion.
    mov rax, 0
    call printf

    mov rdi, formatoRotaciones          ;Formato de la cadena.
    mov rax, 0                          ;No se usaron registros de punto flotante
    mov rsi, [nDesplazamientos]         ;Numero de desplazamientos en formato int
    call printf                         ;Se llama la funcion externa 'printf.'

    add rsp, 8                          ;Re-establecer la pila.
    pop rsi                             ;Re-establecer el registo rsi.
    pop rax                             ;Re-establecer el registo rax.
    pop rdi                             ;Re-establecer el registo rdi.
    ;--------------------------------------------------------------------------

    ;Verificar los argumentos.
    ;--------------------------------------------------------------------------
    
    mov rcx, [rsi+16]
    mov rax, "--crypt"
    cmp [rcx], rax
    je jmp_crypt

    mov rax, "--decrypt"
    cmp [rcx], rax
    je jmp_decrypt

    jmp exit_error

jmp_crypt:
    call encriptar
    jmp exito

jmp_decrypt:
    call desencriptar
    jmp exito

    ;--------------------------------------------------------------------------    

    ;Todo salio con exito!
    ;--------------------------------------------------------------------------
exito:

    push rdi                            ;Guardar el valor del registro rdi.
    push rax                            ;Guardar el valor del registro rax.
    push rsi                            ;Guardar el valor del registro rsi.
    sub rsp, 8                          ;Alinear la pila.

    mov rdi, strLimite                  ;Imprimir el borde punteado.
    mov rax, 0
    call printf

    add rsp, 8                          ;Re-establecer la pila.
    pop rsi                             ;Re-establecer el registo rsi.
    pop rax                             ;Re-establecer el registo rax.
    pop rdi                             ;Re-establecer el registo rdi.

    mov rsp, rbp
    pop rbp
    ret

    ;--------------------------------------------------------------------------

    ;Funciones.
    ;--------------------------------------------------------------------------

encriptar:
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    push rdi                            ;Guardar el valor del registro rdi.
    push rax                            ;Guardar el valor del registro rax.
    push rsi                            ;Guardar el valor del registro rsi.
    sub rsp, 8                          ;Alinear la pila.


    mov rdi, formatoTextoCrypt          ;Formato del texto plano.
    mov rax, 0                          ;No se usaron registros de punto flotante
    mov rsi, [mensaje]
    call printf

    add rsp, 8                          ;Re-establecer la pila.
    pop rsi                             ;Re-establecer el registo rsi.
    pop rax                             ;Re-establecer el registo rax.
    pop rdi                             ;Re-establecer el registo rdi.

    mov rsp, rbp
    pop rbp
    ret

desencriptar:
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    push rdi                            ;Guardar el valor del registro rdi.
    push rax                            ;Guardar el valor del registro rax.
    push rsi                            ;Guardar el valor del registro rsi.
    sub rsp, 8                          ;Alinear la pila.

    mov rdi, formatoTextoDecrypt          ;Formato del texto plano.
    mov rax, 0                          ;No se usaron registros de punto flotante
    mov rsi, [mensaje]
    call printf

    add rsp, 8                          ;Re-establecer la pila.
    pop rsi                             ;Re-establecer el registo rsi.
    pop rax                             ;Re-establecer el registo rax.
    pop rdi                             ;Re-establecer el registo rdi.

    mov rsp, rbp
    pop rbp
    ret

    ;--------------------------------------------------------------------------

    ;Salida en caso de que haya un error.
    ;--------------------------------------------------------------------------

exit_error:                             ;Salida con error.
    mov rax, 1
    mov rdi, STDERR_FILENO              ;Llamada de systema (write), que escribe al
    mov rsi, fewArgumentsError          ;descriptor de archivos stderr.
    mov rdx, fewArgumentsErrorLen
    syscall                             ;Se imprime mensaje de error.

    push rdi                            ;Guardar el valor del registro rdi.
    push rax                            ;Guardar el valor del registro rax.
    push rsi                            ;Guardar el valor del registro rsi.
    sub rsp, 8                          ;Alinear la pila.

    mov rdi, strLimite                  ;Imprimir el borde punteado.
    mov rax, 0
    call printf

    add rsp, 8                          ;Re-establecer la pila.
    pop rsi                             ;Re-establecer el registo rsi.
    pop rax                             ;Re-establecer el registo rax.
    pop rdi                             ;Re-establecer el registo rdi.

    mov rsp, rbp
    pop rbp 

    mov rax, 60                         ;Se sale del programa de manera insatisfactoria
    mov rdi,-1                          ;con valor de retorno de -1.
    syscall
    
    ;--------------------------------------------------------------------------