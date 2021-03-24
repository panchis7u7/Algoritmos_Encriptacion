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
    formatoRotaciones db 'El numero de desplazamientos es %d.',0xA,0x0
    formatoTexto db "El mensaje a encriptar es: %s.",0xA,0x0
    fewArgumentsError db "Error en los argumentos!.",0xA,0x0
    fewArgumentsErrorLen equ $-fewArgumentsError
    STDOUT_FILENO equ 0x01
    STDERR_FILENO equ 0x02
section .bss
    argc resq 1
    nDesplazamientos resb 1
    mensaje resb 1
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
    ;mov r8, [rsi+16]
    ;mov [mensaje], r8

    mov rdi, formatoRotaciones          ;Formato de la cadena.
    mov rax, 0                          ;No se usaron registros de punto flotante
    mov rsi, [nDesplazamientos]         ;Numero de desplazamientos en formato int
    call printf                         ;Se llama la funcion externa 'printf.'

    ;mov rdi, formatoTexto               ;Formato del texto plano.
    ;mov rax, 0                          ;No se usaron registros de punto flotante
    ;mov rsi, [mensaje]
    ;call printf

    mov rsp, rbp
    pop rbp
    ret

    ;--------------------------------------------------------------------------
exit_error:                             ;Salida con error.
    mov rax, 1
    mov rdi, STDERR_FILENO              ;Llamada de systema (write), que escribe al
    mov rsi, fewArgumentsError          ;descriptor de archivos stderr.
    mov rdx, fewArgumentsErrorLen
    syscall                             ;Se imprime mensaje de error.

    mov rsp, rbp
    pop rbp 

    mov rax, 60                         ;Se sale del programa de manera insatisfactoria
    mov rdi,-1                          ;con valor de retorno de -1.
    syscall